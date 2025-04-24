#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/GAS/Common/Ability/Utility/GA_BaseInputAbility.h"
#include "GA_StartCookingMode.generated.h"

class UAT_StartCookingModeCinematic;
class ULevelSequence;

UCLASS()
class PROJECTISG_API UGA_StartCookingMode : public UGA_BaseInputAbility
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
	TObjectPtr<ULevelSequence> StartCookingCinematic;

	UPROPERTY()
	TObjectPtr<UAT_StartCookingModeCinematic> AT_StartCookingModeCinematic;

	UFUNCTION()
	void OnEndCinematic();
};
