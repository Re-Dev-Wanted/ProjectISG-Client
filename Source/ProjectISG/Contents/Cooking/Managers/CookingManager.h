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
	static TArray<FFoodRecipe> GetRecipeData() { return RecipeData; }

private:
	// 
	static TMap<uint32, TArray<FFoodRecipe>> RecipeMapByFoodId;
	static TArray<FFoodRecipe> RecipeData;
	static bool IsInitialize;
};
