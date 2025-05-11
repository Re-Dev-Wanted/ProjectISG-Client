#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "KitchenFurnitureQTEComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTISG_API UKitchenFurnitureQTEComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UKitchenFurnitureQTEComponent();

	void StartQTE();

private:
	uint8 CurrentQTEIndex = 0;

	GETTER_SETTER(uint8, CurrentQTEIndex)

	void PlayNextCookingQTE();
};
