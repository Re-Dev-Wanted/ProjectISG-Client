#include "UIC_QuestItemWidget.h"

#include "UIM_QuestItemWidget.h"
#include "UIV_QuestItemWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/UI/Base/Components/UIManageComponent.h"
#include "ProjectISG/Core/UI/Popup/Diary/UI/QuestList/UIC_QuestListUI.h"
#include "ProjectISG/Systems/QuestStory/Manager/QuestStoryManager.h"

void UUIC_QuestItemWidget::InitializeData(const FString& QuestId)
{
	const FQuestStoryData Quest = UQuestStoryManager::GetQuestDataById(QuestId);

	Cast<UUIV_QuestItemWidget>(GetView())->GetQuestTitle()->SetText(
		FText::FromString(Quest.GetQuestTitle()));

	Cast<UUIV_QuestItemWidget>(GetView())->
		GetQuestItemButton()->OnClicked.AddDynamic(
			this, &ThisClass::OnClickQuestItemWidget);

	Cast<UUIM_QuestItemWidget>(GetModel())->SetCurrentQuestId(QuestId);
}

void UUIC_QuestItemWidget::OnClickQuestItemWidget()
{
	UUIC_QuestListUI* QuestListUIController = Cast<UUIC_QuestListUI>(
		GetView()->GetOwningPlayer<AMainPlayerController>()->
		           GetUIManageComponent()
		           ->ControllerInstances[EUIName::Popup_QuestListUI]);

	QuestListUIController->SetQuestInfo(
		Cast<UUIM_QuestItemWidget>(GetModel())->GetCurrentQuestId());
}
