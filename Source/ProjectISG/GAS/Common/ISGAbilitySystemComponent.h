#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "ISGAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags,
                                    const FGameplayTagContainer& /*AssetTags*/);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTISG_API UISGAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UISGAbilitySystemComponent();

	void Initialize(const class UISGAbilitySystemInitializeData* InitialData);

	GETTER_SETTER(bool, IsInitialize)

	void AbilityActorInfoSet();

	FEffectAssetTags EffectAssetTags;

	void AddCharacterAbilities(
		const TArray<TSubclassOf<class UGameplayAbility>>& StartupAbilities);

protected:
	void EffectApplied(class UAbilitySystemComponent* AbilitySystemComponent,
	                   const FGameplayEffectSpec& EffectSpec,
	                   FActiveGameplayEffectHandle ActiveGameplayEffectHandle);

private:
	bool IsInitialize = false;
};
