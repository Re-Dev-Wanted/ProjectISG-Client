// Fill out your copyright notice in the Description page of Project Settings.

#include "GA_ReelInLine.h"

#include "ProjectISG/Contents/Fishing/Props/FishingRod.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/InteractionComponent.h"
#include "ProjectISG/Core/Character/Player/Component/PlayerHandSlotComponent.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"

void UGA_ReelInLine::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
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

	AMainPlayerController* PC = Player->GetController<AMainPlayerController>();

	PC->SetIgnoreLookInput(false);
	PC->SetIgnoreMoveInput(false);

	Player->GetHandSlotComponent()->SetIsUseInputAction(false);
	

	if (!TriggerEventData->Target)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	if (const AFishingRod* ConstActor = Cast<AFishingRod>(TriggerEventData->Target))
	{
		AFishingRod* FishingRod = const_cast<AFishingRod*>(ConstActor);
		FishingRod->ReelIn();
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UGA_ReelInLine::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
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
			Player->GetInteractionComponent()->SetIsInteractive(true);
		}
	);
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
