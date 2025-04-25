#include "UIC_FoodInfoWidget.h"

#include "UIV_FoodInfoWidget.h"
#include "Components/Image.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"

void UUIC_FoodInfoWidget::SetFoodThumbnail(const uint32 FoodId)
{
	UTexture2D* Thumbnail = UItemManager::GetItemInfoById(FoodId).GetThumbnail()
		.LoadSynchronous();
	Cast<UUIV_FoodInfoWidget>(GetView())->GetFoodThumbnail()->
	                                      SetBrushFromTexture(Thumbnail);
}
