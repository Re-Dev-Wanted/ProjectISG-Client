#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/GAS/Common/Ability/Utility/GA_BaseInputAbility.h"
#include "GA_Watering.generated.h"

class UPlayMontageWithEvent;

UCLASS()
class PROJECTISG_API UGA_Watering : public UGA_BaseInputAbility
{
	GENERATED_BODY()

protected:
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

private:
	UPROPERTY()
	UPlayMontageWithEvent* AT_WateringAnim;

	UFUNCTION()
	void OnEndWateringAnim(FGameplayTag EventTag, FGameplayEventData EventData);

	void LoggingToWatering();
};
