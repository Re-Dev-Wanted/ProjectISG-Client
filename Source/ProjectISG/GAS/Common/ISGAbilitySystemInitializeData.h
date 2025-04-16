#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "ISGAbilitySystemInitializeData.generated.h"

class UGA_BaseInputAbility;

UCLASS()
class PROJECTISG_API UISGAbilitySystemInitializeData : public UDataAsset
{
	GENERATED_BODY()

public:
	GETTER(TArray<TSubclassOf<UGA_BaseInputAbility>>, DefaultGameplayAbilities)
	GETTER
	(FGameplayTagContainer
	 ,
	 DefaultGameplayTags
	)

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<UGA_BaseInputAbility>> DefaultGameplayAbilities;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	FGameplayTagContainer DefaultGameplayTags;
};
