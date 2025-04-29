// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Watering.h"

#include "ProjectISG/Contents/Farming/BaseCrop.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/InteractionComponent.h"
#include "ProjectISG/GAS/Common/Ability/Utility/PlayMontageWithEvent.h"
#include "ProjectISG/Utils/EnumUtil.h"

void UGA_Watering::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                   const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo
                                   ActivationInfo,
                                   const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		ActorInfo->AvatarActor.Get());
	UE_LOG(LogTemp, Warning, TEXT("물주기, %s"), *FEnumUtil::GetClassEnumKeyAsString(Player->GetLocalRole()));


	if (!IsValid(Player)) return;

	AT_WateringAnim = UPlayMontageWithEvent::InitialEvent(this, NAME_None, Player->GetWateringMontage(), FGameplayTagContainer());
	AT_WateringAnim->Activate();
	BlockInputForMontage(true);
	AT_WateringAnim->OnCompleted.AddDynamic(this, &ThisClass::OnEndWateringAnim);
}

void UGA_Watering::EndAbility(const FGameplayAbilitySpecHandle Handle,
                              const FGameplayAbilityActorInfo* ActorInfo,
                              const FGameplayAbilityActivationInfo
                              ActivationInfo,
                              bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility,
	                  bWasCancelled);
}

void UGA_Watering::OnEndWateringAnim(FGameplayTag EventTag, FGameplayEventData EventData)
{
	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
	CurrentActorInfo->AvatarActor.Get());
	ABaseCrop* Crop = Cast<ABaseCrop>(Player->GetInteractionComponent()->GetTargetTraceResult().GetActor());
	Crop->CanInteractive = true;
	
	BlockInputForMontage(false);
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
