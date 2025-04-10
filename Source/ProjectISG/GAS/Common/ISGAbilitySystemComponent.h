#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "ISGAbilitySystemComponent.generated.h"

class UISGAbilitySystemInitializeData;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTISG_API UISGAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UISGAbilitySystemComponent();

	void Initialize(const UISGAbilitySystemInitializeData* InitialData);

	GETTER_SETTER(bool, IsInitialize)

private:
	bool IsInitialize = false;
};
