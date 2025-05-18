#include "UIV_WorkbenchUI.h"

#include "UIM_WorkbenchUI.h"
#include "ProjectISG/Core/UI/Popup/Crafting/Views/ItemListView.h"

void UUIV_WorkbenchUI::OnSetup(const TArray<FCraftingRecipeUIModel>& Array,
	FOnSelectedCraftingRecipe OnSelected)
{
	RecipeListView->OnSetup(Array, OnSelected);
}

void UUIV_WorkbenchUI::OnUpdateUI(FCraftingRecipeUIModel SelectedModel, const TMap<uint16, uint16>& OwningCounts)
{
	MaterialsView->OnUpdateUI(SelectedModel.ItemName, SelectedModel.GetRequiredMaterialsArray(), OwningCounts);
}
