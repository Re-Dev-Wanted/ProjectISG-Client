// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Watering.h"

#include "ProjectISG/Contents/Farming/BaseCrop.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Utils/EnumUtil.h"

void UGA_Watering::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                   const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo
                                   ActivationInfo,
                                   const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	const AMainPlayerCharacter* player = Cast<AMainPlayerCharacter>(
		ActorInfo->AvatarActor.Get());
	UE_LOG(LogTemp, Warning, TEXT("물주기, %s"), *FEnumUtil::GetClassEnumKeyAsString(player->GetLocalRole()));

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
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
