#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "ISGAbilitySystemComponent.generated.h"

class UISGAbilitySystemInitializeData;

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags,
                                    const FGameplayTagContainer& /*AssetTags*/);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTISG_API UISGAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UISGAbilitySystemComponent();

	void Initialize(const UISGAbilitySystemInitializeData* InitialData);

	GETTER_SETTER(bool, IsInitialize)

	void AbilityActorInfoSet();

	FEffectAssetTags EffectAssetTags;

	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);

protected:
	void EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
	                   const FGameplayEffectSpec& EffectSpec,
	                   FActiveGameplayEffectHandle ActiveGameplayEffectHandle);

private:
	bool IsInitialize = false;
};
