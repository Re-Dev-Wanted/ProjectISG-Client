#include "UIM_WorkbenchUI.h"

#include "ProjectISG/Contents/Crafting/CraftingRecipe.h"
#include "ProjectISG/Contents/Crafting/Managers/CraftingManager.h"
#include "ProjectISG/Systems/Inventory/ItemData.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"

void UUIM_WorkbenchUI::LoadAll()
{
	TArray<FCraftingRecipe> DataArray = UCraftingManager::GetRecipes();
	
	for (int32 i = 0, Count = DataArray.Num(); i < Count; i++)
	{
		FCraftingRecipe Data = DataArray[i];
		
		FItemInfoData ItemInfoData = UItemManager::GetItemInfoById(Data.GetItemId());

		FCraftingRecipeUIModel Model;

		Model.Id = Data.GetRecipeId();
		Model.ItemId = Data.GetItemId();
		Model.Thumbnail = ItemInfoData.GetThumbnail();
		Model.ItemName = ItemInfoData.GetDisplayName();
		Model.RequiredMaterials = LoadMaterialsModel(Data.GetRequiredMaterials());

		Recipes.Add(Model);
	}
}

FCraftingRecipeUIModel UUIM_WorkbenchUI::GetRecipeUIModel(uint16 RecipeId) const
{
	SelectedRecipeId = RecipeId;
	
	for (int i = 0, Count = Recipes.Num(); i < Count; i++)
	{
		FCraftingRecipeUIModel Model = Recipes[i];

		if (Model.Id == RecipeId)
		{
			return Model;
		}
	}
	
	return FCraftingRecipeUIModel();
}

TMap<uint16, FCraftingMaterialUIModel> UUIM_WorkbenchUI::LoadMaterialsModel(TMap<uint16, uint16> Map)
{
	TMap<uint16, FCraftingMaterialUIModel> Result;

	for (TPair Pair : Map)
	{
		FItemInfoData ItemInfoData = UItemManager::GetItemInfoById(Pair.Key);

		FCraftingMaterialUIModel Model;
		Model.Id = Pair.Key;
		Model.Name = ItemInfoData.GetDisplayName();
		Model.Thumbnail = ItemInfoData.GetThumbnail();
		Model.RequiredCount = Pair.Value;

		Result.Add(Pair.Key, Model);
	}

	return Result;
}
