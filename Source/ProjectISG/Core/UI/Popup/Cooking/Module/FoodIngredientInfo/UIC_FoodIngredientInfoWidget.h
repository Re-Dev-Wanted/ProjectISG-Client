#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "UIC_FoodIngredientInfoWidget.generated.h"

UCLASS()
class PROJECTISG_API UUIC_FoodIngredientInfoWidget : public UBaseUIController
{
	GENERATED_BODY()

public:
	void SetIngredientInfo(const uint32 IngredientId,
	                       const uint32 IngredientCount);
};
