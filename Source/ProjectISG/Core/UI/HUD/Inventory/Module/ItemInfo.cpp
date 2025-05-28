#include "ItemInfo.h"
#include "Components/Overlay.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Components/MultiLineEditableTextBox.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"

void UItemInfo::ShowItemData(const FItemMetaInfo& ItemMetaInfo)
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
	const FItemInfoData ItemInfoData = UItemManager::GetItemInfoById(
		ItemMetaInfo.GetId());

	ItemDisplayName->SetText(FText::FromString(ItemInfoData.GetDisplayName()));
	ItemRank->SetText(
		FText::FromString(UItemManager::GetItemGradeText(ItemMetaInfo)));
	ItemDescription->SetText(ItemInfoData.GetDescription());

	IsActive = true;
}

void UItemInfo::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (IsActive)
	{
		const FVector2d CurrentMousePosition =
			UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

		UCanvasPanelSlot* SlotPanel = UWidgetLayoutLibrary::SlotAsCanvasSlot(
			ItemPanel);

		SlotPanel->SetPosition(CurrentMousePosition);
	}
}

void UItemInfo::RemoveItemWidget()
{
	SetVisibility(ESlateVisibility::Hidden);
	IsActive = false;
}
