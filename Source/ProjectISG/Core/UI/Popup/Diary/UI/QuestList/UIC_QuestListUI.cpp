#include "UIC_QuestListUI.h"

#include "AsyncTreeDifferences.h"
#include "UIM_QuestListUI.h"
#include "UIV_QuestListUI.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/MultiLineEditableTextBox.h"
#include "Components/ListView.h"
#include "Components/Overlay.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Core/UI/Popup/Diary/Widget/QuestItem/QuestItemWidgetObject.h"
#include "ProjectISG/Core/UI/Popup/Diary/Widget/QuestRequiredText/UIV_QuestRequiredTextWidget.h"
#include "ProjectISG/Core/UI/Popup/Diary/Widget/QuestShowItemInfo/UIC_QuestShowItemInfoWidget.h"
#include "ProjectISG/Core/UI/Popup/Diary/Widget/QuestShowItemInfo/UIV_QuestShowItemInfoWidget.h"
#include "ProjectISG/Systems/Inventory/ItemData.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"
#include "ProjectISG/Systems/QuestStory/Component/QuestManageComponent.h"
#include "ProjectISG/Systems/QuestStory/Manager/QuestStoryManager.h"
#include "ProjectISG/Systems/Time/TimeManager.h"
#include "ProjectISG/Utils/SessionUtil.h"

void UUIC_QuestListUI::InitializeController(UBaseUIView* NewView
											, UBaseUIModel* NewModel)
{
	Super::InitializeController(NewView, NewModel);

	const UUIV_QuestListUI* QuestListUIView = Cast<UUIV_QuestListUI>(GetView());
	QuestListUIView->GetQuestButton()->OnClicked.AddDynamic(
		this, &ThisClass::OnClickQuestButton);
}

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

	InitializeQuestList();
}

void UUIC_QuestListUI::SetQuestInfo(const FString& QuestId)
{
	UUIM_QuestListUI* QuestListUIModel = Cast<UUIM_QuestListUI>(GetModel());

	QuestListUIModel->SetCurrentSelectQuestId(QuestId);

	SetQuestInfoData(QuestId);
	SetQuestRequireData(QuestId);
	SetQuestRewardData(QuestId);
}

void UUIC_QuestListUI::InitializeQuestList()
{
	const UUIV_QuestListUI* QuestListUIView = Cast<UUIV_QuestListUI>(GetView());

	QuestListUIView->GetQuestListView()->ClearListItems();

	for (FQuestStoryData& CurrentQuestData :
		UQuestStoryManager::GetAllQuestData())
	{
		// 보이지 않는 조건에 대한 처리
		if (CurrentQuestData.GetQuestMetaData().
							Contains(EQuestStoryMetaDataKey::IsHideInQuestBook)
			&& CurrentQuestData.GetQuestMetaData()[
				EQuestStoryMetaDataKey::IsHideInQuestBook] == TEXT("true"))
		{
			break;
		}

		UQuestItemWidgetObject* QuestItemWidgetObject = NewObject<
			UQuestItemWidgetObject>();
		QuestItemWidgetObject->QuestId = CurrentQuestData.GetQuestId();

		QuestListUIView->GetQuestListView()->AddItem(QuestItemWidgetObject);
	}
}

void UUIC_QuestListUI::SetQuestInfoData(const FString& QuestId)
{
	const UUIV_QuestListUI* QuestListUIView = Cast<UUIV_QuestListUI>(GetView());
	AMainPlayerController* PC = QuestListUIView->GetOwningPlayer<
		AMainPlayerController>();
	const FQuestStoryData QuestData = UQuestStoryManager::GetQuestDataById(
		QuestId);

	QuestListUIView->GetQuestInfo()->SetVisibility(ESlateVisibility::Visible);

	QuestListUIView->GetQuestTitle()->SetText(
		FText::FromString(QuestData.GetQuestTitle()));

	QuestListUIView->GetQuestTitle()->SetText(
		FText::FromString(QuestData.GetQuestTitle()));

	QuestListUIView->GetQuestScenario()->SetText(QuestData.GetQuestScenario());

	QuestListUIView->GetQuestHint()->SetText(
		FText::FromString(QuestData.GetQuestHint()));

	switch (PC->GetQuestManageComponent()->GetQuestStatusById(QuestId))
	{
	case EQuestStatus::Available:
		{
			// 서버만이 퀘스트 시작, 종료를 실행할 수 있다.
			QuestListUIView->GetQuestButton()->SetIsEnabled(PC->HasAuthority());
			QuestListUIView->GetQuestText()->SetText(
				FText::FromString(TEXT("퀘스트 시작")));

			break;
		}
	case EQuestStatus::Unavailable:
		{
			QuestListUIView->GetQuestButton()->SetIsEnabled(false);
			QuestListUIView->GetQuestText()->SetText(
				FText::FromString(TEXT("진행 불가")));

			break;
		}
	case EQuestStatus::InProgress:
		{
			QuestListUIView->GetQuestButton()->SetIsEnabled(false);
			QuestListUIView->GetQuestText()->SetText(
				FText::FromString(TEXT("진행 중")));

			break;
		}
	case EQuestStatus::CanComplete:
		{
			// 서버만이 퀘스트 시작, 종료를 실행할 수 있다.
			QuestListUIView->GetQuestButton()->SetIsEnabled(PC->HasAuthority());
			QuestListUIView->GetQuestText()->SetText(
				FText::FromString(TEXT("완료 가능")));

			break;
		}
	case EQuestStatus::Completed:
		{
			QuestListUIView->GetQuestButton()->SetIsEnabled(false);
			QuestListUIView->GetQuestText()->SetText(
				FText::FromString(TEXT("완료됨")));

			break;
		}
	default:
		{
			break;
		}
	}
}

void UUIC_QuestListUI::SetQuestRequireData(const FString& QuestId)
{
	const UUIV_QuestListUI* QuestListUIView = Cast<UUIV_QuestListUI>(GetView());
	QuestListUIView->GetQuestRequireList()->ClearChildren();

	for (FQuestRequireData& RequireQuest :
		UQuestStoryManager::GetRequireQuestDataById(QuestId))
	{
		switch (RequireQuest.GetRequireType())
		{
		case EQuestRequireType::HasItem:
			{
				SetQuestRequireItemData(RequireQuest);
				break;
			}
		default:
			{
				break;
			}
		}
	}
}

void UUIC_QuestListUI::SetQuestRequireItemData(
	FQuestRequireData& RequireQuest) const
{
	UUIV_QuestListUI* QuestListUIView = Cast<UUIV_QuestListUI>(GetView());

	const AMainPlayerState* PS = Cast<AMainPlayerState>(
		GetView()->GetOwningPlayerState());

	const uint32 ItemId = RequireQuest.GetRequireItemOptions().GetItemId();

	const uint32 RequireCount = RequireQuest.GetRequireItemOptions().
											GetItemCount();

	const FItemInfoData ItemInfoData = UItemManager::GetItemInfoById(ItemId);

	UUIV_QuestRequiredTextWidget* NewWidget = CreateWidget<
		UUIV_QuestRequiredTextWidget>(QuestListUIView
									, QuestListUIView->
									GetQuestRequiredTextClass());

	// 현재 내가 보유하고 있는 아이템 Id 기반의 갯수 가져오기
	const uint32 CurrentItemCount = PS->GetInventoryComponent()->
										GetCurrentRemainItemValue().
										Contains(ItemId)
										? PS->GetInventoryComponent()->
											GetCurrentRemainItemValue()[ItemId]
										: 0;

	NewWidget->SetPadding(FMargin(0, 0, 0, 8));

	NewWidget->GetRequireDescription()->SetText(
		FText::FromString(
			FString::Printf(TEXT("%s 획득"), *ItemInfoData.GetDisplayName())));

	NewWidget->GetRequireStatus()->SetText(FText::FromString(
		FString::Printf(TEXT("%d/%d"), CurrentItemCount, RequireCount)));

	const bool HasDoneQuestRequired = CurrentItemCount >= RequireCount;

	if (HasDoneQuestRequired)
	{
		NewWidget->GetRequireDescription()->SetColorAndOpacity(
			QuestListUIView->GetRequiredQuestHasDoneColor());
		NewWidget->GetRequireStatus()->SetColorAndOpacity(
			QuestListUIView->GetRequiredQuestHasDoneColor());
	}
	else
	{
		NewWidget->GetRequireDescription()->SetColorAndOpacity(
			QuestListUIView->GetRequiredQuestDefaultColor());
		NewWidget->GetRequireStatus()->SetColorAndOpacity(
			QuestListUIView->GetRequiredQuestDefaultColor());
	}

	QuestListUIView->GetQuestRequireList()->AddChild(NewWidget);
}

void UUIC_QuestListUI::SetQuestRewardData(const FString& QuestId)
{
	const UUIV_QuestListUI* QuestListUIView = Cast<UUIV_QuestListUI>(GetView());
	QuestListUIView->GetQuestRequireList()->ClearChildren();

	TArray<FQuestRewardData> QuestRewardList =
		UQuestStoryManager::GetRewardQuestDataById(QuestId);

	if (QuestRewardList.IsEmpty())
	{
		QuestListUIView->GetQuestRewardList()->SetVisibility(
			ESlateVisibility::Hidden);
		return;
	}

	// 기존 보상 Child 초기화
	QuestListUIView->GetQuestRewardListScroll()->ClearChildren();

	for (FQuestRewardData& RewardItem : QuestRewardList)
	{
		UUIV_QuestShowItemInfoWidget* RewardChild = CreateWidget<
			UUIV_QuestShowItemInfoWidget>(GetView()
										, QuestListUIView->
										GetQuestRewardItemClass());

		RewardChild->SetPadding({0, 0, 4, 0});

		QuestListUIView->GetQuestRewardListScroll()->AddChild(RewardChild);
		Cast<UUIC_QuestShowItemInfoWidget>(RewardChild->GetController())->
			SetShowItemInfo(RewardItem);
	}
}

void UUIC_QuestListUI::OnClickQuestButton()
{
	const UUIV_QuestListUI* QuestListUIView = Cast<UUIV_QuestListUI>(GetView());
	const UUIM_QuestListUI* QuestListUIModel = Cast<UUIM_QuestListUI>(
		GetModel());

	AMainPlayerController* PC = QuestListUIView->GetOwningPlayer<
		AMainPlayerController>();

	const FQuestStoryData QuestData = UQuestStoryManager::GetQuestDataById(
		QuestListUIModel->GetCurrentSelectQuestId());

	switch (PC->GetQuestManageComponent()->GetQuestStatusById(
		QuestListUIModel->GetCurrentSelectQuestId()))
	{
	case EQuestStatus::Available:
		{
			PopUIFromPlayerController();
			PC->StartQuest(QuestListUIModel->GetCurrentSelectQuestId());

			break;
		}
	case EQuestStatus::CanComplete:
		{
			PopUIFromPlayerController();
			PC->EndQuest();

			break;
		}
	default:
		{
			break;
		}
	}
}
