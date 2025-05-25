#include "UIC_QuestItemWidget.h"

#include "UIM_QuestItemWidget.h"
#include "UIV_QuestItemWidget.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/UI/Base/Components/UIManageComponent.h"
#include "ProjectISG/Core/UI/Popup/Diary/UI/QuestList/UIC_QuestListUI.h"
#include "ProjectISG/Systems/QuestStory/Component/QuestManageComponent.h"
#include "ProjectISG/Systems/QuestStory/Manager/QuestStoryManager.h"

void UUIC_QuestItemWidget::InitializeData(const FString& QuestId)
{
	const FQuestStoryData Quest = UQuestStoryManager::GetQuestDataById(QuestId);

	// View, Model Casting
	Cast<UUIM_QuestItemWidget>(GetModel())->SetCurrentQuestId(QuestId);

	const UUIV_QuestItemWidget* QuestItemWidget = Cast<UUIV_QuestItemWidget>(
		GetView());

	// 퀘스트 관련 정보 가져오기
	const AMainPlayerController* PC = Cast<AMainPlayerController>(
		GetView()->GetOwningPlayer());
	const UQuestManageComponent* QuestManageComponent = PC->
		GetQuestManageComponent();

	// View 관련 정보 설정 코드
	QuestItemWidget->GetQuestTitle()->SetText(
		FText::FromString(Quest.GetQuestTitle()));

	// View 관련 현재 완료 가능한 행동 갯수를 가져오는 코드
	QuestItemWidget->GetCurrentFinishQuestCount()->SetText(
		FText::FromString(FString::FromInt(
			UQuestStoryManager::GetRequireQuestDateToAbleFinish(PC, QuestId))));

	// View 관련 최대 퀘스트 행동 갯수 가져오는 코드
	QuestItemWidget->GetMaxFinishQuestCount()->SetText(
		FText::FromString(FString::FromInt(
			UQuestStoryManager::GetRequireQuestDataById(QuestId).Num())));

	QuestItemWidget->GetQuestItemButton()->OnClicked.AddDynamic(
		this, &ThisClass::OnClickQuestItemWidget);

	switch (QuestManageComponent->GetQuestStatusById(QuestId))
	{
	case EQuestStatus::InProgress:
	case EQuestStatus::CanComplete:
		{
			QuestItemWidget->GetQuestPlayingNotify()->SetVisibility(
				ESlateVisibility::Visible);
			QuestItemWidget->GetQuestStatus()->SetVisibility(
				ESlateVisibility::Hidden);
			break;
		}
	case EQuestStatus::Completed:
		{
			QuestItemWidget->GetQuestPlayingNotify()->SetVisibility(
				ESlateVisibility::Hidden);
			QuestItemWidget->GetQuestStatus()->SetVisibility(
				ESlateVisibility::Visible);
			QuestItemWidget->GetQuestStatus()->SetText(
				FText::FromString(TEXT("완료")));
			break;
		}
	case EQuestStatus::Available:
		{
			QuestItemWidget->GetQuestPlayingNotify()->SetVisibility(
				ESlateVisibility::Hidden);
			QuestItemWidget->GetQuestStatus()->SetVisibility(
				ESlateVisibility::Visible);
			QuestItemWidget->GetQuestStatus()->SetText(
				FText::FromString(TEXT("진행 가능")));
			break;
		}
	case EQuestStatus::Unavailable:
		{
			QuestItemWidget->GetQuestPlayingNotify()->SetVisibility(
				ESlateVisibility::Hidden);
			QuestItemWidget->GetQuestStatus()->SetVisibility(
				ESlateVisibility::Visible);
			QuestItemWidget->GetQuestStatus()->SetText(
				FText::FromString(TEXT("미완")));
			break;
		}
	default: { break; }
	}
}

void UUIC_QuestItemWidget::OnClickQuestItemWidget()
{
	UUIC_QuestListUI* QuestListUIController = Cast<UUIC_QuestListUI>(
		GetView()->GetOwningPlayer<AMainPlayerController>()->
					GetUIManageComponent()->ControllerInstances[
			EUIName::Popup_QuestListUI]);

	QuestListUIController->SetQuestInfo(
		Cast<UUIM_QuestItemWidget>(GetModel())->GetCurrentQuestId());
}
