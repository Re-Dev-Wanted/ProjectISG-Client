// Fill out your copyright notice in the Description page of Project Settings.

#include "GA_ReelInLine.h"

#include "Kismet/KismetSystemLibrary.h"
#include "ProjectISG/Contents/Fishing/Props/FishActor.h"
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

void UGA_ReelInLine::PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData)
{
	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);

	AT_SuccessFishingCinematic =
				UAT_SuccessFishingCinematic::InitialEvent(
					this, SuccessFishingCinematic);

	AT_FailFishingCinematic = UAT_FailFishingCinematic::InitialEvent(
				this, FailFishingCinematic);
}

void UGA_ReelInLine::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo,
                                     const FGameplayAbilityActivationInfo
                                     ActivationInfo,
                                     const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);


	AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
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
			AT_SuccessFishingCinematic->OnSuccessFishingCinematicEndNotified.
								AddUniqueDynamic(this, &UGA_ReelInLine::OnEndCinematic);
			AT_SuccessFishingCinematic->ReadyForActivation();

			TWeakObjectPtr<AMainPlayerCharacter> WeakPlayer = Player;
			TWeakObjectPtr<AFishingRod> WeakFishingRod = FishingRod;
			TWeakObjectPtr<ThisClass> WeakThis = this;

			GetWorld()->GetTimerManager().SetTimer
			(
				TimerHandle,
				[WeakThis, WeakPlayer, WeakFishingRod]()
				{
					if (WeakThis.IsValid() && WeakPlayer.IsValid() && WeakFishingRod.IsValid())
					{
						WeakThis.Get()->FishActor = WeakThis.Get()->GetWorld()
						->SpawnActor<AFishActor>(WeakThis.Get()->FishActorFactory);

						FFishData FishData = WeakFishingRod.Get()->GetFishData();

						WeakThis.Get()->FishActor->SetMesh(FishData.GetMesh());

						WeakThis.Get()->FishActor->AttachToComponent(WeakPlayer.Get()->GetMesh(), 
						FAttachmentTransformRules
						::SnapToTargetNotIncludingScale, *FishData.GetSocketName());
					}
				},
				1.f,
				false
			);
		}
		else
		{
			AT_FailFishingCinematic->OnFailFishingCinematicEndNotified.
							 AddUniqueDynamic(this, &UGA_ReelInLine::OnEndCinematic);
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
	AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		ActorInfo->AvatarActor.Get());

	if (!Player)
	{
		Super::EndAbility(Handle, ActorInfo, ActivationInfo,
		                  bReplicateEndAbility, bWasCancelled);
		return;
	}

	TWeakObjectPtr<AMainPlayerCharacter> WeakPlayer = Player;
	GetWorld()->
		GetTimerManager()
		.SetTimerForNextTick
		(
			[WeakPlayer]()
			{
				if (WeakPlayer.IsValid())
				{
					WeakPlayer.Get()->GetInteractionComponent()->SetIsInteractive(true);
				}
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

	if (FishActor)
	{
		FishActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		FishActor->Destroy();
		FishActor = nullptr;
	}

	if (TimerHandle.IsValid())
	{
		TimerHandle.Invalidate();
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
