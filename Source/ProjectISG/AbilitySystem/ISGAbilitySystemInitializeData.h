#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "Common/Utils/MacroUtil.h"
#include "ISGAbilitySystemInitializeData.generated.h"

class UAttributeSet;
class UBaseInputAbility;

UCLASS()
class PROJECTISG_API UISGAbilitySystemInitializeData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	GETTER(TArray<TSubclassOf<UBaseInputAbility>>, DefaultGameplayAbilities)
	GETTER(FGameplayTagContainer, DefaultGameplayTags)

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<UBaseInputAbility>> DefaultGameplayAbilities;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	FGameplayTagContainer DefaultGameplayTags;
};