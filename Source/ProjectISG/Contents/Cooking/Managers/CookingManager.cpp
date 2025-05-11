#include "CookingManager.h"

bool UCookingManager::IsInitialize;
TArray<FFoodRecipe> UCookingManager::RecipeData;
TMap<uint32, TArray<FFoodRecipe>> UCookingManager::RecipeMapByFoodId;

void UCookingManager::Initialize()
{
	if (IsInitialize)
	{
		return;
	}

	IsInitialize = true;

	const UDataTable* ItemInfoDataTable = LoadObject<UDataTable>(
		nullptr, TEXT(
			"/Script/Engine.DataTable'/Game/Contents/Cooking/Data/DT_FoodRecipe.DT_FoodRecipe'")
		, nullptr, LOAD_None, nullptr);

	if (!ItemInfoDataTable)
	{
		return;
	}

	TArray<FFoodRecipe*> TempItemInfoList;
	ItemInfoDataTable->GetAllRows<FFoodRecipe>(TEXT(""), TempItemInfoList);

	for (const FFoodRecipe* InfoItem : TempItemInfoList)
	{
		RecipeData.Add(*InfoItem);

		if (!RecipeMapByFoodId.Find(InfoItem->GetFoodId()))
		{
			RecipeMapByFoodId.Add(InfoItem->GetFoodId());
		}

		RecipeMapByFoodId[InfoItem->GetFoodId()].Add(*InfoItem);
	}
}
