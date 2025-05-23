// Fill out your copyright notice in the Description page of Project Settings.

#include "GA_ReelInLine.h"

#include "Kismet/KismetSystemLibrary.h"
#include "ProjectISG/Contents/Fishing/Props/FishingRod.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/InteractionComponent.h"
#include "ProjectISG/Core/Character/Player/Component/PlayerHandSlotComponent.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Core/UI/Base/Components/UIManageComponent.h"
#include "ProjectISG/Systems/Animation/Manager/LevelSequenceManager.h"
#include "Task/AT_FailFishingCinematic.h"
#include "Task/AT_SuccessFishingCinematic.h"

void UGA_ReelInLine::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo,
                                     const FGameplayAbilityActivationInfo
                                     ActivationInfo,
                                     const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);


	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		ActorInfo->AvatarActor.Get());

	if (!Player)
	{
		return;
	}

	TObjectPtr<ABaseActor> HeldItem = Player->GetHandSlotComponent()->
	                                          GetHeldItem();

	if (!HeldItem)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo,
		           true, false);
		return;
	}

	if (const AFishingRod* ConstActor = Cast<AFishingRod>(HeldItem))
	{
		AFishingRod* FishingRod = const_cast<AFishingRod*>(ConstActor);

		FishingRod->ReelInLine();

		if (FishingRod->GetIsBiteFish())
		{
			AT_SuccessFishingCinematic =
				UAT_SuccessFishingCinematic::InitialEvent(
					this, SuccessFishingCinematic);
			AT_SuccessFishingCinematic->OnSuccessFishingCinematicEndNotified.
			                            AddDynamic(
				                            this, &UGA_ReelInLine::
				                            OnEndCinematic);
			AT_SuccessFishingCinematic->ReadyForActivation();
		}
		else
		{
			AT_FailFishingCinematic = UAT_FailFishingCinematic::InitialEvent(
				this, FailFishingCinematic);
			AT_FailFishingCinematic->OnFailFishingCinematicEndNotified.
			                         AddDynamic(
				                         this, &UGA_ReelInLine::OnEndCinematic);
			AT_FailFishingCinematic->ReadyForActivation();
		}

		return;
	}

	OnEndCinematic();
}

void UGA_ReelInLine::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                const FGameplayAbilityActorInfo* ActorInfo,
                                const FGameplayAbilityActivationInfo
                                ActivationInfo, bool bReplicateEndAbility,
                                bool bWasCancelled)
{
	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		ActorInfo->AvatarActor.Get());

	if (!Player)
	{
		Super::EndAbility(Handle, ActorInfo, ActivationInfo,
		                  bReplicateEndAbility, bWasCancelled);
		return;
	}

	GetWorld()->
		GetTimerManager()
		.SetTimerForNextTick
		(
			[Player]()
			{
				Player->GetInteractionComponent()->SetIsInteractive(true);
			}
		);

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility,
	                  bWasCancelled);
}

void UGA_ReelInLine::OnEndCinematic()
{
	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		CurrentActorInfo->AvatarActor.Get());

	if (Player->IsLocallyControlled())
	{
		AMainPlayerController* PC = Player->GetController<
			AMainPlayerController>();

		PC->SetIgnoreLookInput(false);
		PC->SetIgnoreMoveInput(false);

		PC->GetUIManageComponent()->ResetWidget();
	}

	TObjectPtr<ABaseActor> HeldItem = Player->GetHandSlotComponent()->
	                                          GetHeldItem();

	if (!HeldItem)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo,
		           true, false);
		return;
	}

	if (const AFishingRod* ConstActor = Cast<AFishingRod>(HeldItem))
	{
		AFishingRod* FishingRod = const_cast<AFishingRod*>(ConstActor);

		FishingRod->OnEndReelInLine(CurrentActorInfo->AvatarActor.Get());
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true,
	           false);
}
