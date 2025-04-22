// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Watering.h"

#include "AT_Watering.h"

void UGA_Watering::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                   const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo,
                                   const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 근처 씨앗을 가져온다.
	// 물이 안준 상태이면 실행한다.
	// 아니라면 스킵

	AT_Watering = UAT_Watering::InitialEvent(this);

	AT_Watering->FindSeedDelegate.AddDynamic(this, &ThisClass::FindSeedEndNotified);
	AT_Watering->ReadyForActivation();
	
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UGA_Watering::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility,
	                  bWasCancelled);
}

void UGA_Watering::FindSeedEndNotified()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
