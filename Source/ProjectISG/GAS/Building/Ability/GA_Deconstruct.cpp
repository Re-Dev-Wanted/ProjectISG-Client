// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Deconstruct.h"

#include "Kismet/KismetSystemLibrary.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"

void UGA_Deconstruct::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo,
                                      const FGameplayAbilityActivationInfo ActivationInfo,
                                      const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		ActorInfo->AvatarActor.Get());

	if (!Player)
	{
		return;
	}
	
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("파괴해라"));
}

void UGA_Deconstruct::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility,
	                  bWasCancelled);
}
