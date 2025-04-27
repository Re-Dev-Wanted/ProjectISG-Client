#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIModel.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "UIM_FoodInfoWidget.generated.h"

UCLASS()
class PROJECTISG_API UUIM_FoodInfoWidget : public UBaseUIModel
{
	GENERATED_BODY()

public:
	GETTER_SETTER(uint32, RecipeId)

private:
	uint32 RecipeId;
};
