// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_WakeUp.h"

#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/GAS/Common/Ability/Utility/PlayMontageWithEvent.h"
#include "ProjectISG/Utils/EnumUtil.h"

void UGA_WakeUp::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                 const FGameplayAbilityActorInfo* ActorInfo,
                                 const FGameplayAbilityActivationInfo
                                 ActivationInfo,
                                 const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		ActorInfo->AvatarActor.Get());
	if (!IsValid(Player))
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("침대에서 일어나기, %s"),
	       *FEnumUtil::GetClassEnumKeyAsString(Player->GetLocalRole()));

	AT_WakeUpAnim = UPlayMontageWithEvent::InitialEvent
	(
		this,
		NAME_None,
		Player->GetWakeUpMontage(),
		FGameplayTagContainer(),
		*TriggerEventData);

	AT_WakeUpAnim->Activate();
	AT_WakeUpAnim->OnCompleted.AddDynamic(
		this, &ThisClass::OnCompleteWakeUpMontage);
}

void UGA_WakeUp::EndAbility(const FGameplayAbilitySpecHandle Handle,
                            const FGameplayAbilityActorInfo* ActorInfo,
                            const FGameplayAbilityActivationInfo ActivationInfo,
                            bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility,
	                  bWasCancelled);
}

void UGA_WakeUp::OnCompleteWakeUpMontage(FGameplayTag EventTag,
                                         FGameplayEventData EventData)
{
	BlockInputForMontage(false);
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo,
	           false, false);
}
