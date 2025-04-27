#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIModel.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "UIM_FoodIngredientInfoWidget.generated.h"

UCLASS()
class PROJECTISG_API UUIM_FoodIngredientInfoWidget : public UBaseUIModel
{
	GENERATED_BODY()

public:
	GETTER_SETTER(uint32, IngredientId)
	GETTER_SETTER(uint32, IngredientCount)

private:
	uint32 IngredientId;

	uint32 IngredientCount;
};
