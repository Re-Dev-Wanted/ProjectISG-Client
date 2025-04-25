#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ProjectISG/Contents/Cooking/CookingStruct.h"
#include "CookingManager.generated.h"

UCLASS()
class PROJECTISG_API UCookingManager : public UObject
{
	GENERATED_BODY()

public:
	static void Initialize();
	static TMap<uint32, FFoodRecipe> GetRecipeData() { return RecipeData; }

private:
	static TMap<uint32, FFoodRecipe> RecipeData;
};
