#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/GAS/Common/Ability/Utility/GA_BaseInputAbility.h"
#include "GameplayTagContainer.h"
#include "GA_Seeding.generated.h"

UCLASS()
class PROJECTISG_API UGA_Seeding : public UGA_BaseInputAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	                             const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo
	                             ActivationInfo,
	                             const FGameplayEventData*
	                             TriggerEventData) override;
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION()
	void CreateSeed(FGameplayTag EventTag, FGameplayEventData EventData);

	void LoggingToSeeding();

	UPROPERTY()
	class UPlayMontageWithEvent* AT_SeedingAnim = nullptr;

	UPROPERTY(EditAnywhere, Category = "Options",
		meta = (AllowPrivateAccess = true))
	FGameplayTag CueTag;
	
};
