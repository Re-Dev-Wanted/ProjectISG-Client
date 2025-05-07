// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/GAS/Common/Ability/Utility/GA_BaseInputAbility.h"
#include "GA_CastBobber.generated.h"

class UAT_StartFishingCinematic;
class ULevelSequence;
/**
 * 
 */
UCLASS()
class PROJECTISG_API UGA_CastBobber : public UGA_BaseInputAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UPROPERTY(EditDefaultsOnly, Category = Cinematic)
	TObjectPtr<ULevelSequence> StartFishingCinematic;

	UPROPERTY()
	TObjectPtr<UAT_StartFishingCinematic> AT_StartFishingCinematic;

	UFUNCTION()
	void OnEndCinematic();
};
