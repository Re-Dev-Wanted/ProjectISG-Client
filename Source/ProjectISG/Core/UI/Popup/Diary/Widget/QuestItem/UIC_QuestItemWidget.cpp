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

	// View 관련 최대 퀘스트 행동 갯수 가져오는 코드
	QuestItemWidget->GetMaxFinishQuestCount()->SetText(
		FText::FromString(
			FString::FromInt(
				UQuestStoryManager::GetQuestAllBehaviorCount(QuestId))));

	QuestItemWidget->GetQuestItemButton()->OnClicked.AddDynamic(
		this, &ThisClass::OnClickQuestItemWidget);

	// 현재 보여지는 위젯의 QuestId가 현재 진행중인 퀘스트인 경우에 대한 대응
	if (QuestManageComponent->GetCurrentPlayingQuestId() == QuestId)
	{
		QuestItemWidget->GetQuestPlayingNotify()->SetVisibility(
			ESlateVisibility::Visible);
		QuestItemWidget->GetQuestStatus()->SetVisibility(
			ESlateVisibility::Hidden);
	}
	else
	{
		QuestItemWidget->GetQuestPlayingNotify()->SetVisibility(
			ESlateVisibility::Hidden);
		QuestItemWidget->GetQuestStatus()->SetVisibility(
			ESlateVisibility::Visible);
		// 현재 진행중이지 않은 퀘스트에 대해 완료인지, 아직 수행하지 않았는지는
		// QuestManageComponent에 있는 완료된 Set 정보에 값이 들어있는지
		// 확인하고 수행해준다.
		if (QuestManageComponent->GetEndQuestIdList().Contains(QuestId))
		{
			QuestItemWidget->GetQuestStatus()->SetText(
				FText::FromString(TEXT("완료")));
		}
		else
		{
			QuestItemWidget->GetQuestStatus()->SetText(
				FText::FromString(TEXT("미완")));
		}
	}
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
