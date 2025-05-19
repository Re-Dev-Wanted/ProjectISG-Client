#include "UIC_QuestListUI.h"

#include "UIV_QuestListUI.h"
#include "Components/TextBlock.h"
#include "Components/MultiLineEditableTextBox.h"
#include "Components/ListView.h"
#include "Components/Overlay.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Core/UI/Popup/Diary/Widget/QuestItem/QuestItemWidgetObject.h"
#include "ProjectISG/Systems/QuestStory/Manager/QuestStoryManager.h"
#include "ProjectISG/Systems/Time/TimeManager.h"
#include "ProjectISG/Utils/SessionUtil.h"

void UUIC_QuestListUI::AppearUI()
{
	Super::AppearUI();

	const UUIV_QuestListUI* QuestListUIView = Cast<UUIV_QuestListUI>(GetView());

	const ATimeManager* TimeManager = QuestListUIView->GetOwningPlayerState<
		AMainPlayerState>()->GetTimeManager();

	QuestListUIView->GetCurrentDateTime()->SetText(
		FText::FromString(TimeManager->GetDateDisplayText()));

	QuestListUIView->GetUserId()->SetText(
		FText::FromString(FSessionUtil::GetCurrentId(GetWorld())));
}

void UUIC_QuestListUI::InitializeController(UBaseUIView* NewView,
                                            UBaseUIModel* NewModel)
{
	Super::InitializeController(NewView, NewModel);

	const UUIV_QuestListUI* QuestListUIView = Cast<UUIV_QuestListUI>(GetView());

	for (FQuestStoryData CurrentQuestData :
	     UQuestStoryManager::GetAllQuestData())
	{
		UQuestItemWidgetObject* QuestItemWidgetObject = NewObject<
			UQuestItemWidgetObject>();
		QuestItemWidgetObject->QuestId = CurrentQuestData.GetQuestId();

		QuestListUIView->GetQuestListView()->AddItem(QuestItemWidgetObject);
	}
}

void UUIC_QuestListUI::SetQuestInfo(const FString& QuestId)
{
	const UUIV_QuestListUI* QuestListUIView = Cast<UUIV_QuestListUI>(GetView());
	const FQuestStoryData QuestData = UQuestStoryManager::GetQuestDataById(
		QuestId);

	QuestListUIView->GetQuestInfo()->SetVisibility(ESlateVisibility::Visible);
	QuestListUIView->GetQuestTitle()->SetText(
		FText::FromString(QuestData.GetQuestTitle()));
	QuestListUIView->GetQuestTitle()->SetText(
		FText::FromString(QuestData.GetQuestTitle()));
	QuestListUIView->GetQuestScenario()->SetText(QuestData.GetQuestScenario());
}
