#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/GAS/Common/Ability/Utility/GA_BaseInputAbility.h"
#include "GA_OnGoingCrafting.generated.h"

class UPlayMontageWithEvent;

UCLASS()
class PROJECTISG_API UGA_OnGoingCrafting : public UGA_BaseInputAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void EndMontage(FGameplayTag EventTag, FGameplayEventData Payload);

	UPROPERTY()
	UPlayMontageWithEvent* AT_StartMontageEvent;
};
