#include "MaterialsView.h"

#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "ProjectISG/Core/UI/Popup/Crafting/UI/UIM_WorkbenchUI.h"
#include "ProjectISG/Core/UI/Popup/Crafting/Widget/MaterialInfoWidget.h"
#include "ProjectISG/Utils/AsyncUtil.h"

void UMaterialsView::NativePreConstruct()
{
	Super::NativePreConstruct();

	ScrollBox->ClearChildren();

	for (int8 i = 0; i < DebugCount; i++)
	{
		UMaterialInfoWidget* Widget = CreateWidget<UMaterialInfoWidget>(this, WidgetFactory);
		ScrollBox->AddChild(Widget);
	}
}

void UMaterialsView::NativeConstruct()
{
	Super::NativeConstruct();

	// 처음에는 가린다.
	SetVisibility(ESlateVisibility::Hidden);
}

void UMaterialsView::OnUpdateUI(const FString& ItemName, const TArray<FCraftingMaterialUIModel>& Materials)
{
	SelectedItemName->SetText(FText::FromString(ItemName));
	
	ScrollBox->ClearChildren();
	
	for (int32 i = 0, Count = Materials.Num(); i < Count; i++)
	{
		FCraftingMaterialUIModel Model = Materials[i];

		UMaterialInfoWidget* Widget = CreateWidget<UMaterialInfoWidget>(this, WidgetFactory);

		AsyncUtil::LoadAsync<UTexture2D>
		(
			Model.Thumbnail,
			[this,Widget, Model](UTexture2D* Thumbnail)
			{
				Widget->SetWidget(Model.Id, Model.RequiredCount, Model.Name, Thumbnail);
			}
		);

		ScrollBox->AddChild(Widget);
	}

	SetVisibility(ESlateVisibility::Visible);
}
