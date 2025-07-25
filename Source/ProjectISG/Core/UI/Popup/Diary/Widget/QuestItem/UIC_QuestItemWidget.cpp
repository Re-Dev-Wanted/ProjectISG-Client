﻿#include "UIC_QuestItemWidget.h"

#include "UIM_QuestItemWidget.h"
#include "UIV_QuestItemWidget.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/UI/Base/Components/UIManageComponent.h"
#include "ProjectISG/Core/UI/Popup/Diary/UI/QuestList/UIC_QuestListUI.h"
#include "ProjectISG/Core/UI/Popup/Diary/Widget/QuestShowItemInfo/UIC_QuestShowItemInfoWidget.h"
#include "ProjectISG/Core/UI/Popup/Diary/Widget/QuestShowItemInfo/UIV_QuestShowItemInfoWidget.h"
#include "ProjectISG/Systems/QuestStory/Component/QuestManageComponent.h"
#include "ProjectISG/Systems/QuestStory/Manager/QuestStoryManager.h"

void UUIC_QuestItemWidget::InitializeData(const FString& QuestId)
{
	SetQuestDefaultInformation(QuestId);
	SetQuestRewardList(QuestId);
	SetQuestStatus(QuestId);
}

void UUIC_QuestItemWidget::AppearUI()
{
	Super::AppearUI();
}

void UUIC_QuestItemWidget::OnClickQuestItemWidget()
{
	UUIC_QuestListUI* QuestListUIController = Cast<UUIC_QuestListUI>(
		GetView()->GetOwningPlayer<AMainPlayerController>()->
		           GetUIManageComponent()->ControllerInstances[
			EUIName::Popup_QuestListUI]);

	if (QuestListUIController->GetSelectedQuestWidget())
	{
		UUIC_QuestItemWidget* PrevWidget = Cast<UUIC_QuestItemWidget>(
			QuestListUIController->GetSelectedQuestWidget()->GetController());
		PrevWidget->SetDefaultWidget();
	}

	QuestListUIController->SetQuestInfo(
		Cast<UUIM_QuestItemWidget>(GetModel())->GetCurrentQuestId());
	QuestListUIController->SetSelectedQuestWidget(
		Cast<UUIV_QuestItemWidget>(GetView()));

	SetSelectedWidget();
}

void UUIC_QuestItemWidget::SetDefaultWidget()
{
	UUIV_QuestItemWidget* QuestItemWidgetView = Cast<
		UUIV_QuestItemWidget>(
		GetView());

	QuestItemWidgetView->GetQuestItemButton()->SetRenderScale({1, 1});
	QuestItemWidgetView->GetQuestBorder()->SetBrushTintColor(
		QuestItemWidgetView->GetDefaultColor());
}

void UUIC_QuestItemWidget::SetSelectedWidget()
{
	UUIV_QuestItemWidget* QuestItemWidgetView = Cast<
		UUIV_QuestItemWidget>(
		GetView());

	QuestItemWidgetView->GetQuestItemButton()->SetRenderScale({
		QuestItemWidgetView->GetSelectedScale(),
		QuestItemWidgetView->GetSelectedScale()
	});
	QuestItemWidgetView->GetQuestBorder()->SetBrushTintColor(
		QuestItemWidgetView->GetSelectedColor());
}

void UUIC_QuestItemWidget::SetQuestDefaultInformation(const FString& QuestId)
{
	const FQuestStoryData Quest = UQuestStoryManager::GetQuestDataById(QuestId);

	// View, Model Casting
	Cast<UUIM_QuestItemWidget>(GetModel())->SetCurrentQuestId(QuestId);

	const UUIV_QuestItemWidget* QuestItemWidget = Cast<UUIV_QuestItemWidget>(
		GetView());

	// 퀘스트 관련 정보 가져오기
	const AMainPlayerController* PC = Cast<AMainPlayerController>(
		GetView()->GetOwningPlayer());

	const uint16 CurrentQuestToClear =
		UQuestStoryManager::GetRequireQuestDateToAbleFinish(PC, QuestId);
	const uint16 RequireQuestToClear =
		UQuestStoryManager::GetRequireQuestDataById(QuestId).Num();
	const EQuestStatus QuestStatus = PC->GetQuestManageComponent()->
	                                     GetQuestStatusById(QuestId);

	// View 관련 정보 설정 코드
	QuestItemWidget->GetQuestTitle()->SetText(
		FText::FromString(Quest.GetQuestTitle()));

	// View 관련 현재 완료 가능한 행동 갯수를 가져오는 코드
	// 이미 완료된 퀘스트면 예외 없이 해야 하는 행동 수로 설정해준다.
	QuestItemWidget->GetCurrentFinishQuestCount()->SetText(
		FText::FromString(FString::FromInt(
			QuestStatus == EQuestStatus::Completed
				? RequireQuestToClear
				: CurrentQuestToClear)));

	// View 관련 최대 퀘스트 행동 갯수 가져오는 코드
	QuestItemWidget->GetMaxFinishQuestCount()->SetText(
		FText::FromString(FString::FromInt(RequireQuestToClear)));

	QuestItemWidget->GetQuestItemButton()->OnClicked.AddUniqueDynamic(
		this, &ThisClass::OnClickQuestItemWidget);
}

void UUIC_QuestItemWidget::SetQuestRewardList(const FString& QuestId) const
{
	const UUIV_QuestItemWidget* QuestItemWidget = Cast<UUIV_QuestItemWidget>(
		GetView());

	// 보상 미리보기 UI를 위한 로직
	QuestItemWidget->GetRewardPreviewList()->ClearChildren();

	for (FQuestRewardData& RewardItem :
	     UQuestStoryManager::GetRewardQuestDataById(QuestId))
	{
		UUIV_QuestShowItemInfoWidget* RewardChild = CreateWidget<
			UUIV_QuestShowItemInfoWidget>(GetView()
			                              , QuestItemWidget->
			                              GetQuestRewardItemClass());

		RewardChild->SetPadding({0, 0, 4, 0});
		RewardChild->InitializeMVC();

		QuestItemWidget->GetRewardPreviewList()->AddChild(RewardChild);

		Cast<UUIC_QuestShowItemInfoWidget>(RewardChild->GetController())->
			SetShowItemInfo(RewardItem);
	}
}

void UUIC_QuestItemWidget::SetQuestStatus(const FString& QuestId) const
{
	const UUIV_QuestItemWidget* QuestItemWidget = Cast<UUIV_QuestItemWidget>(
		GetView());
	// 현재 퀘스트의 상태 보여주는 로직
	const AMainPlayerController* PC = Cast<AMainPlayerController>(
		GetView()->GetOwningPlayer());

	switch (PC->GetQuestManageComponent()->GetQuestStatusById(QuestId))
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
				FText::FromString(TEXT("진행 불가")));
			break;
		}
	default: { break; }
	}
}
