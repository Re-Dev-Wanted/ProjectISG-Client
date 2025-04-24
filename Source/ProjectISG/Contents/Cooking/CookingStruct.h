#pragma once

#include "CookingStruct.generated.h"

enum class ECookingTool : uint8;

USTRUCT(BlueprintType)
struct PROJECTISG_API FCookingRecipeFlow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<uint16> RecipeFlow;
};

USTRUCT(BlueprintType)
struct PROJECTISG_API FFoodRecipe : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<uint16, uint8> RecipeData;

	UPROPERTY()
	ECookingTool CookingTool;
};
