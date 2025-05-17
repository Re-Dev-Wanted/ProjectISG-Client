#include "CraftingManager.h"

bool UCraftingManager::IsInitialized;
TArray<FCraftingRecipe> UCraftingManager::Recipes;
TMap<uint16, FCraftingRecipe> UCraftingManager::RecipeMapByItemId;

void UCraftingManager::Initialize()
{
	if (IsInitialized)
	{
		return;
	}

	IsInitialized = true;

	const static ConstructorHelpers::FObjectFinder<UDataTable> CraftingTable(TEXT("/Game/Contents/Crafting/Data/DT_CraftingRecipe.DT_CraftingRecipe"));

	if (CraftingTable.Succeeded())
	{
		TArray<FCraftingRecipe*> Temp;

		CraftingTable.Object->GetAllRows<FCraftingRecipe>(TEXT(""), Temp);

		for (const FCraftingRecipe* RowData : Temp)
		{
			Recipes.Add(*RowData);

			uint16 Key = RowData->GetItemId();
			
			if (!RecipeMapByItemId.Contains(Key))
			{
				RecipeMapByItemId.Add(Key);
			}

			RecipeMapByItemId[Key] = *RowData;
		}
	}
}
