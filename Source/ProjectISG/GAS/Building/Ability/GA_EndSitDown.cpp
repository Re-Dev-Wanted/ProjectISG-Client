// Fill out your copyright notice in the Description page of Project Settings.

#include "GA_EndSitDown.h"

#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/InteractionComponent.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/GAS/Common/Ability/Utility/PlayMontageWithEvent.h"
#include "ProjectISG/Systems/Grid/Actors/Placement.h"

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
		FGameplayEventData()
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

	Player->bUseControllerRotationYaw = true;
	Player->GetCharacterMovement()->bOrientRotationToMovement = false;
	
	Player->GetController()->SetIgnoreMoveInput(false);
	Player->GetInteractionComponent()->SetIsInteractive(true);

	if (CurrentEventData.Target)
	{
		const AActor* Target = CurrentEventData.Target;
		const APlacement* ConstPlacement = Cast<APlacement>(Target);
	
		APlacement* Placement = const_cast<APlacement*>(ConstPlacement);
	
		AActor* Actor = CurrentActorInfo->AvatarActor.Get();
		
		Actor->AttachToComponent(Placement->MeshComp,
		FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("SitSocket"));
		
		ConstPlacement->SetCollisionEnabled(true);
	}
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
