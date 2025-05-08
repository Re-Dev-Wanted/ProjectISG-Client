// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/GAS/Common/Ability/Utility/GA_BaseInputAbility.h"
#include "GA_StartSitDown.generated.h"


class UInputMappingContext;

UCLASS()
class PROJECTISG_API UGA_StartSitDown : public UGA_BaseInputAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = AnimMontage)
	TObjectPtr<UAnimMontage> StartSitDownMontage;

	UPROPERTY(EditDefaultsOnly, Category = InputBinding)
	TObjectPtr<UInputAction> InputAction;

	UPROPERTY(EditDefaultsOnly, Category = InputBinding)
	TObjectPtr<UInputMappingContext> IMC;
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION()
	void NotifyMontage(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void EndMontage();

	UFUNCTION()
	void OnStartedInputEvent();

	void BindInputAction(UEnhancedInputComponent* EnhancedInputComponent);

	UPROPERTY()
	class UPlayMontageWithEvent* AT_StartMontageEvent;
	
};
