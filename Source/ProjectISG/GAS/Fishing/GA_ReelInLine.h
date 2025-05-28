// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/GAS/Common/Ability/Utility/GA_BaseInputAbility.h"
#include "GA_ReelInLine.generated.h"

class UAT_FailFishingCinematic;
class UAT_SuccessFishingCinematic;
class ULevelSequence;

UCLASS()
class PROJECTISG_API UGA_ReelInLine : public UGA_BaseInputAbility
{
	GENERATED_BODY()

protected:
	virtual void PreActivate(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate,
		const FGameplayEventData* TriggerEventData = nullptr) override;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	                             const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo
	                             ActivationInfo,
	                             const FGameplayEventData*
	                             TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle,
	                        const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo,
	                        bool bReplicateEndAbility,
	                        bool bWasCancelled) override;

	UPROPERTY(EditDefaultsOnly, Category = "Options|Cinematic",
		meta = (AllowPrivateAccess = true))
	TObjectPtr<ULevelSequence> SuccessFishingCinematic;

	UPROPERTY(EditDefaultsOnly, Category = "Options|Cinematic",
		meta = (AllowPrivateAccess = true))
	TObjectPtr<ULevelSequence> FailFishingCinematic;

	UPROPERTY()
	TObjectPtr<UAT_SuccessFishingCinematic> AT_SuccessFishingCinematic;

	UPROPERTY()
	TObjectPtr<UAT_FailFishingCinematic> AT_FailFishingCinematic;

public:
	UFUNCTION()
	void OnEndCinematic();
};
