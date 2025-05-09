// Fill out your copyright notice in the Description page of Project Settings.

#include "GA_EndSitDown.h"

#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/InteractionComponent.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/GAS/Common/Ability/Utility/PlayMontageWithEvent.h"
#include "ProjectISG/Systems/Grid/Actors/Placement.h"
#include "ProjectISG/Systems/Grid/Components/PlacementIndicatorComponent.h"

void UGA_EndSitDown::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                     const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AT_EndMontageEvent = UPlayMontageWithEvent::InitialEvent
	(
		this,
		NAME_None,
		EndSitDownMontage,
		FGameplayTagContainer(),
		*TriggerEventData
	);

	AT_EndMontageEvent->OnCompleted.AddDynamic(this, &UGA_EndSitDown::EndMontage);

	AT_EndMontageEvent->ReadyForActivation();
}

void UGA_EndSitDown::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_EndSitDown::EndMontage(FGameplayTag EventTag, FGameplayEventData EventData)
{
	AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(CurrentActorInfo->AvatarActor.Get());
	
	if (!Player)
	{
		return;
	}

	if (Player->IsLocallyControlled())
	{
		Player->bUseControllerRotationYaw = true;
		Player->GetCharacterMovement()->bOrientRotationToMovement = false;
		Player->GetInteractionComponent()->SetIsInteractive(true);
		Player->GetPlacementIndicatorComponent()->SetIsActive(true);
	}
	
	if (EventData.Target)
	{
		const AActor* Target = EventData.Target.Get();
		const APlacement* ConstPlacement = Cast<APlacement>(Target);
		
		ConstPlacement->Multicast_SetCollisionEnabled(true);
	}
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
