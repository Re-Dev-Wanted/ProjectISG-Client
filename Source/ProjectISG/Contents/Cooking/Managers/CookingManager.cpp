#include "CookingManager.h"

TMap<uint32, FFoodRecipe> UCookingManager::RecipeData;

void UCookingManager::Initialize()
{
	const static ConstructorHelpers::FObjectFinder<UDataTable>
		ItemInfoDataTable(TEXT(
			"/Script/Engine.DataTable'/Game/Contents/Cooking/Data/DT_FoodRecipe.DT_FoodRecipe'"));

	if (ItemInfoDataTable.Succeeded())
	{
		TArray<FFoodRecipe*> TempItemInfoList;
		ItemInfoDataTable.Object->GetAllRows<FFoodRecipe>(
			TEXT(""), TempItemInfoList);

		for (const FFoodRecipe* InfoItem : TempItemInfoList)
		{
			RecipeData.Add(InfoItem->GetFoodId(), *InfoItem);
		}
	}
}
