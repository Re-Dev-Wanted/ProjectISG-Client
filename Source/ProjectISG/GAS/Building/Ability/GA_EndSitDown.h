// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/GAS/Common/Ability/Utility/GA_BaseInputAbility.h"
#include "GA_EndSitDown.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTISG_API UGA_EndSitDown : public UGA_BaseInputAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = AnimMontage)
	TObjectPtr<UAnimMontage> EndSitDownMontage;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UPROPERTY()
	class UPlayMontageWithEvent* AT_EndMontageEvent;

	UFUNCTION()
	void EndMontage(FGameplayTag EventTag, FGameplayEventData EventData);
};
