#include "UIC_QuestListUI.h"

#include "UIV_QuestListUI.h"
#include "Components/TextBlock.h"
#include "Components/MultiLineEditableTextBox.h"
#include "Components/ListView.h"
#include "Components/Overlay.h"
#include "Components/VerticalBox.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Core/UI/Popup/Diary/Widget/QuestItem/QuestItemWidgetObject.h"
#include "ProjectISG/Core/UI/Popup/Diary/Widget/QuestRequiredText/UIV_QuestRequiredTextWidget.h"
#include "ProjectISG/Systems/Inventory/ItemData.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"
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

	InitializeQuestList();
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

	QuestListUIView->GetQuestHint()->SetText(
		FText::FromString(QuestData.GetQuestHint()));

	SetQuestRequireData(QuestId);
}

void UUIC_QuestListUI::InitializeQuestList()
{
	const UUIV_QuestListUI* QuestListUIView = Cast<UUIV_QuestListUI>(GetView());

	QuestListUIView->GetQuestListView()->ClearListItems();

	for (FQuestStoryData& CurrentQuestData :
	     UQuestStoryManager::GetAllQuestData())
	{
		if (CurrentQuestData.GetQuestMetaData().Contains(EQuestStoryMetaDataKey::IsHideInQuestBook)
			&& CurrentQuestData.GetQuestMetaData()[EQuestStoryMetaDataKey::IsHideInQuestBook] == TEXT("true"))
		{
			break;
		}
		
		UQuestItemWidgetObject* QuestItemWidgetObject = NewObject<
			UQuestItemWidgetObject>();
		QuestItemWidgetObject->QuestId = CurrentQuestData.GetQuestId();

		QuestListUIView->GetQuestListView()->AddItem(QuestItemWidgetObject);
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

	const uint32 ItemId = RequireQuest.GetRequireItemOptions().
	                                   GetRequireItemId();

	const uint32 RequireCount = RequireQuest.GetRequireItemOptions().
	                                         GetRequireItemCount();

	const FItemInfoData ItemInfoData =
		UItemManager::GetItemInfoById(ItemId);

	UUIV_QuestRequiredTextWidget* NewWidget = CreateWidget<
		UUIV_QuestRequiredTextWidget>(
		QuestListUIView,
		QuestListUIView->GetQuestRequiredTextClass());

	// 현재 내가 보유하고 있는 아이템 Id 기반의 갯수 가져오기
	const uint32 CurrentItemCount = PS->GetInventoryComponent()->
	                                    GetCurrentRemainItemValue().
	                                    Contains(ItemId)
		                                ? PS->
		                                  GetInventoryComponent()->
		                                  GetCurrentRemainItemValue()[
			                                ItemId]
		                                : 0;

	NewWidget->SetPadding(FMargin(0, 0, 0, 8));

	NewWidget->GetRequireDescription()->SetText(
		FText::FromString(
			FString::Printf(
				TEXT("%s 획득"), *ItemInfoData.GetDisplayName())));

	NewWidget->GetRequireStatus()->SetText(
		FText::FromString(
			FString::Printf(
				TEXT("%d/%d"), CurrentItemCount, RequireCount)));

	const bool HasDoneQuestRequired = CurrentItemCount >=
		RequireCount;

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
