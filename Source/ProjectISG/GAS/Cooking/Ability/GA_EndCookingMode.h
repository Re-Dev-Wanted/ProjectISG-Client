#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/GAS/Common/Ability/Utility/GA_BaseInputAbility.h"
#include "GA_EndCookingMode.generated.h"

class UAT_EndCookingModeCinematic;
class ULevelSequence;

UCLASS()
class PROJECTISG_API UGA_EndCookingMode : public UGA_BaseInputAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	                             const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo
	                             ActivationInfo,
	                             const FGameplayEventData*
	                             TriggerEventData) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Options|Cinematic",
		meta = (AllowPrivateAccess = true))
	TObjectPtr<ULevelSequence> EndCookingCinematic;

	UPROPERTY()
	TObjectPtr<UAT_EndCookingModeCinematic> AT_EndCookingModeCinematic;

	UFUNCTION()
	void OnEndCinematic();

	void UnlockPlayer();
};
