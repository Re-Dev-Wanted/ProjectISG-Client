#include "UIC_QuestShowItemInfoWidget.h"

#include "UIV_QuestShowItemInfoWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"
#include "ProjectISG/Systems/QuestStory/QuestStoryData.h"

void UUIC_QuestShowItemInfoWidget::SetShowItemInfo(FQuestRewardData& RewardData)
{
	const UUIV_QuestShowItemInfoWidget* QuestShowItemInfoWidgetView = Cast<
		UUIV_QuestShowItemInfoWidget>(GetView());

	const FItemInfoData ItemInfoData = UItemManager::GetItemInfoById(
		RewardData.GetRewardItemOptions().GetItemId());

	QuestShowItemInfoWidgetView->GetIngredientThumbnail()->
	                             SetBrushFromSoftTexture(
		                             ItemInfoData.GetThumbnail());
	QuestShowItemInfoWidgetView->GetCurrentIngredientCount()->SetText(
		FText::AsNumber(RewardData.GetRewardItemOptions().GetItemCount()));
}
