// Fill out your copyright notice in the Description page of Project Settings.

#include "GA_CastBobber.h"

#include "ProjectISG/Contents/Fishing/Props/FishingRod.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/InteractionComponent.h"
#include "ProjectISG/Core/Character/Player/Component/PlayerHandSlotComponent.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "Task/AT_StartFishingCinematic.h"

class AMainPlayerState;

void UGA_CastBobber::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                     const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		ActorInfo->AvatarActor.Get());

	if (!Player)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	const TObjectPtr<ABaseActor> Equipment = Player->GetHandSlotComponent()->GetHeldItem();

	if (!Equipment)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	FHitResult TargetTraceResult = Player->GetInteractionComponent()->GetTargetTraceResult();

	if (AFishingRod* FishingRod = Cast<AFishingRod>(Equipment))
	{
		FishingRod->StartCasting(ActorInfo->AvatarActor.Get(), TargetTraceResult.ImpactPoint);

		AMainPlayerController* PC = Player->GetController<AMainPlayerController>();

		PC->SetIgnoreLookInput(true);
		PC->SetIgnoreMoveInput(true);

		Player->GetInteractionComponent()->SetIsInteractive(false);

		AT_StartFishingCinematic = UAT_StartFishingCinematic::InitialEvent(this, StartFishingCinematic);
		AT_StartFishingCinematic->OnStartFishingCinematicEndNotified.AddDynamic(this, &UGA_CastBobber::OnEndCinematic);
		AT_StartFishingCinematic->ReadyForActivation();
		return;
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UGA_CastBobber::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(ActorInfo->AvatarActor.Get());
	
	if (!Player)
	{
		Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
		return;
	}

	GetWorld()->
	GetTimerManager()
	.SetTimerForNextTick
	(
		[Player]()
		{
			Player->GetHandSlotComponent()->SetIsUseInputAction(true);
		}
	);
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_CastBobber::OnEndCinematic()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
