#include "ItemListGroup.h"

#include "Components/HorizontalBox.h"
#include "ProjectISG/Core/UI/Popup/Crafting/UI/UIM_WorkbenchUI.h"
#include "ProjectISG/Core/UI/Popup/Crafting/Widget/ItemInfoWidget.h"
#include "ProjectISG/Utils/AsyncUtil.h"

void UItemListGroup::SetGroup(int32 GroupIndex, const TArray<FCraftingRecipeUIModel>& Recipes, FOnSelectedCraftingRecipe OnSelectedRecipe)
{
	ItemGroupBox->ClearChildren();

	Index = GroupIndex;

	int32 Count = Recipes.Num();

	for (int32 i = 0; i < Count; i++)
	{
		FCraftingRecipeUIModel Model = Recipes[i];
		
		UItemInfoWidget* Widget = CreateWidget<UItemInfoWidget>(this, WidgetFactory);

		AsyncUtil::LoadAsync<UTexture2D>
		(
			Model.Thumbnail,
			[this, Widget, Model, OnSelectedRecipe](UTexture2D* Thumbnail)
			{
				Widget->SetWidget(Model.Id, Model.ItemName, Thumbnail, OnSelectedRecipe);
			}
		);
		
		ItemGroupBox->AddChild(Widget);

	}
}

void UItemListGroup::NativePreConstruct()
{
	Super::NativePreConstruct();

	// ItemGroupBox->ClearChildren();
	//
	// for (int8 i = 0; i < DebugCount; i++)
	// {
	// 	UItemInfoWidget* Widget = CreateWidget<UItemInfoWidget>(this, WidgetFactory);
	// 	ItemGroupBox->AddChild(Widget);
	// }
}
