#include "UIC_FoodInfoWidget.h"

#include "UIV_FoodInfoWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"

void UUIC_FoodInfoWidget::SetFoodInfo(const uint32 FoodId)
{
	UUIV_FoodInfoWidget* FoodInfoWidget = Cast<UUIV_FoodInfoWidget>(GetView());

	const FItemInfoData ItemInfoData = UItemManager::GetItemInfoById(FoodId);
	UTexture2D* Thumbnail = ItemInfoData.GetThumbnail()
	                                    .LoadSynchronous();
	FoodInfoWidget->GetFoodThumbnail()->
	                SetBrushFromTexture(Thumbnail);
	FoodInfoWidget->GetFoodName()->SetText(
		FText::FromString(ItemInfoData.GetDisplayName()));
}
