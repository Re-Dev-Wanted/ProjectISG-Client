﻿#include "MaterialsView.h"

#include "Components/Image.h"
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
		UMaterialInfoWidget* Widget = CreateWidget<UMaterialInfoWidget>(
			this, WidgetFactory);
		ScrollBox->AddChild(Widget);
	}
}

void UMaterialsView::NativeConstruct()
{
	Super::NativeConstruct();

	// 처음에는 가린다.
	SetVisibility(ESlateVisibility::Hidden);
}

void UMaterialsView::OnUpdateUI(const FString& ItemName,
                                const TArray<FCraftingMaterialUIModel>&
                                Materials,
                                const TMap<uint16, uint16>& OwningCounts)
{
	SelectedItemName->SetText(FText::FromString(ItemName));

	ScrollBox->ClearChildren();

	for (int32 i = 0, Count = Materials.Num(); i < Count; i++)
	{
		FCraftingMaterialUIModel Model = Materials[i];

		UMaterialInfoWidget* Widget = CreateWidget<UMaterialInfoWidget>(
			this, WidgetFactory);

		uint32 OwningCount = OwningCounts.Find(Model.Id)
			                     ? OwningCounts[Model.Id]
			                     : 0;
		Widget->SetWidget(Model.Id, OwningCount, Model.RequiredCount,
		                  Model.Name);

		AsyncUtil::LoadAsync<UTexture2D>
		(
			Model.Thumbnail,
			[this, Widget](UTexture2D* Thumbnail)
			{
				Widget->SetImage(Thumbnail);
			}
		);

		ScrollBox->AddChild(Widget);
	}

	SetVisibility(ESlateVisibility::Visible);
}

void UMaterialsView::SetDescription(const FText& Desc)
{
	ItemDesc->SetText(Desc);
}

void UMaterialsView::SetImage(TSoftObjectPtr<UTexture2D> Thumbnail)
{
	AsyncUtil::LoadAsync<UTexture2D>
	(
		Thumbnail,
		[this](UTexture2D* Texture)
		{
			ItemIcon->SetBrushFromTexture(Texture);
		}
	);
}
