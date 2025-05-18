#include "ItemListView.h"

#include "ItemListGroup.h"
#include "Components/ScrollBox.h"
#include "ProjectISG/Core/UI/Popup/Crafting/UI/UIM_WorkbenchUI.h"
#include "ProjectISG/Utils/ArrayUtil.h"

void UItemListView::NativePreConstruct()
{
	Super::NativePreConstruct();

	ScrollBox->ClearChildren();

	for (uint8 i = 0; i < DebugCount; i++)
	{
		UItemListGroup* Group = CreateWidget<UItemListGroup>(this, ItemFactory);
		ScrollBox->AddChild(Group);
	}
}

void UItemListView::OnSetup(const TArray<FCraftingRecipeUIModel>& Recipes, FOnSelectedCraftingRecipe OnSelected)
{
	ScrollBox->ClearChildren();

	int32 Count = Recipes.Num();
	int32 GroupCount = FMath::CeilToInt(Count / static_cast<float>(GroupingCount));

	for (int32 i = 0; i < GroupCount; i++)
	{
		int32 StartIndex = i * GroupingCount;
		TArray<FCraftingRecipeUIModel> SubRecipes = ArrayUtil::SubArrayRange(Recipes, StartIndex, GroupingCount);
		
		UItemListGroup* Group = CreateWidget<UItemListGroup>(this, ItemFactory);
		Group->SetGroup(i, SubRecipes, OnSelected);
		
		ScrollBox->AddChild(Group);
	}
}

