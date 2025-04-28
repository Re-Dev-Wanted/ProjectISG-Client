#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/GAS/Common/Ability/Utility/GA_BaseInputAbility.h"
#include "GA_CookingStartQTEEvent.generated.h"

enum class ECookingQTEType : uint8;

UCLASS()
class PROJECTISG_API UGA_CookingStartQTEEvent : public UGA_BaseInputAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle
								, const FGameplayAbilityActorInfo* ActorInfo
								, const FGameplayAbilityActivationInfo
								ActivationInfo
								, const FGameplayEventData*
								TriggerEventData) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Options"
		, meta = (AllowPrivateAccess))
	ECookingQTEType CookingQTE;
};
