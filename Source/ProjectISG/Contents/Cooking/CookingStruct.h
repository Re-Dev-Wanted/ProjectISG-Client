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

	// 실제로는 Item Table에 들어가는 아이디로 Recipe의
	// 고유한 RowName과 별개로 관리한다.
	UPROPERTY()
	uint32 FoodId;

	UPROPERTY()
	TMap<uint16, uint8> RecipeData;

	UPROPERTY()
	ECookingTool CookingTool;
};
