#include "ItemInfo.h"
#include "Components/Overlay.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"

void UItemInfo::ShowItemData(const FItemMetaInfo& ItemMetaInfo,
                             const FItemInfoData& ItemInfoData)
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
	ItemDisplayName->SetText(FText::FromString(ItemInfoData.GetDisplayName()));
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
