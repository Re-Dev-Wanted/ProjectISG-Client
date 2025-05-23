#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/GAS/Common/Ability/Utility/GA_BaseInputAbility.h"
#include "GA_StartCraftingMode.generated.h"

class ULevelSequence;
class UAT_StartCraftingMode;

UCLASS()
class PROJECTISG_API UGA_StartCraftingMode : public UGA_BaseInputAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	                             const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo
	                             ActivationInfo,
	                             const FGameplayEventData*
	                             TriggerEventData) override;

	UPROPERTY()
	TObjectPtr<UAT_StartCraftingMode> AT_StartCraftingModeCinematic;

	UFUNCTION()
	void OnEndCinematic();
};
