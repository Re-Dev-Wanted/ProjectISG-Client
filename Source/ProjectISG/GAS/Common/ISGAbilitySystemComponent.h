#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "ISGAbilitySystemComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTISG_API UISGAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UISGAbilitySystemComponent();

	void Initialize(const class UISGAbilitySystemInitializeData* InitialData);

	GETTER_SETTER(bool, IsInitialize)

private:
	bool IsInitialize = false;
};
