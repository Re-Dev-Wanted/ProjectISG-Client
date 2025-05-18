#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Contents/Crafting/CraftingRecipe.h"
#include "UObject/Object.h"
#include "CraftingManager.generated.h"


UCLASS()
class PROJECTISG_API UCraftingManager : public UObject
{
	GENERATED_BODY()

public:
	static void Initialize();
	static TArray<FCraftingRecipe> GetRecipes() { return Recipes; }

private:
	static TMap<uint16, FCraftingRecipe> RecipeMapByItemId;
	static TArray<FCraftingRecipe> Recipes;
	static bool IsInitialized;
	
};
