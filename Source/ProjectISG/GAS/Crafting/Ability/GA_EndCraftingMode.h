#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/GAS/Common/Ability/Utility/GA_BaseInputAbility.h"
#include "GA_EndCraftingMode.generated.h"

class ULevelSequence;
class UAT_EndCraftingMode;

UCLASS()
class PROJECTISG_API UGA_EndCraftingMode : public UGA_BaseInputAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditDefaultsOnly, Category = "Options|Cinematic",
		meta = (AllowPrivateAccess = true))
	TObjectPtr<ULevelSequence> EndCraftingCinematic;

	UPROPERTY()
	TObjectPtr<UAT_EndCraftingMode> AT_EndCraftingModeCinematic;

	UFUNCTION()
	void OnEndCinematic();
};
