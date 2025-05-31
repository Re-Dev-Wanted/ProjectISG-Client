// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_LieInBed.h"

#include "AbilitySystemComponent.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/InteractionComponent.h"
#include "ProjectISG/Core/Character/Player/Component/PlayerHandSlotComponent.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/GAS/Common/Ability/Utility/PlayMontageWithEvent.h"
#include "ProjectISG/Systems/Animation/Manager/AnimMontageManager.h"
#include "ProjectISG/Systems/Grid/Components/PlacementIndicatorComponent.h"
#include "ProjectISG/Systems/Time/Bed.h"
#include "ProjectISG/Utils/EnumUtil.h"

void UGA_LieInBed::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                   const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo
                                   ActivationInfo,
                                   const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		ActorInfo->AvatarActor.Get());
	if (!IsValid(Player))
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("침대에 눕기 어빌리티, %s"),
	       *FEnumUtil::GetClassEnumKeyAsString(Player->GetLocalRole()));

	Player->bUseControllerRotationYaw = false;
	if (CurrentEventData.Target)
	{
		const AActor* Target = CurrentEventData.Target.Get();
		const ABed* Bed = Cast<ABed>(Target);
		Bed->SetCollisionEnabled(false);
	}

	if (Player->IsLocallyControlled())
	{
		Player->GetInteractionComponent()->SetIsInteractive(false);
		Player->GetPlacementIndicatorComponent()->SetIsActive(false);
	}

	Player->GetHandSlotComponent()->ToggleShowItem(false);

	AT_LyingAnim = UPlayMontageWithEvent::InitialEvent
	(
		this,
		NAME_None,
		UAnimMontageManager::GetMontage(
			Player->GetPlayerState<AMainPlayerState>(),
			EAnimMontageKey::Sleep_Lying),
		FGameplayTagContainer(),
		CurrentEventData);

	AT_LyingAnim->Activate();

	BlockInputForMontage(true);
	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}

void UGA_LieInBed::EndAbility(const FGameplayAbilitySpecHandle Handle,
                              const FGameplayAbilityActorInfo* ActorInfo,
                              const FGameplayAbilityActivationInfo
                              ActivationInfo,
                              bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility,
	                  bWasCancelled);
}
