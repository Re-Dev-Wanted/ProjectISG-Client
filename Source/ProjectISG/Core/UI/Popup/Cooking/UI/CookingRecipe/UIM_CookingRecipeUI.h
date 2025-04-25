#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Contents/Cooking/CookingStruct.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIModel.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "UIM_CookingRecipeUI.generated.h"

UCLASS()
class PROJECTISG_API UUIM_CookingRecipeUI : public UBaseUIModel
{
	GENERATED_BODY()

public:
	GETTER(FFoodRecipe, SelectRecipe)

private:
	UPROPERTY()
	FFoodRecipe SelectRecipe;
};
