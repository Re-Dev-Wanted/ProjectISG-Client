#include "UIC_MainHUD.h"

#include "UIV_MainHUD.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/PlayerHandSlotComponent.h"
#include "ProjectISG/Core/Character/Player/Component/PlayerInventoryComponent.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Core/UI/Gameplay/QuestStory/Widget/AutoQuest/UIC_AutoQuestDialogueWidget.h"
#include "ProjectISG/Core/UI/Gameplay/QuestStory/Widget/AutoQuest/UIV_AutoQuestDialogueWidget.h"
#include "ProjectISG/Core/UI/Gameplay/QuestStory/Widget/CurrentQuest/UIC_CurrentQuestWidget.h"
#include "ProjectISG/Core/UI/Gameplay/QuestStory/Widget/CurrentQuest/UIV_CurrentQuestWidget.h"
#include "ProjectISG/Core/UI/Gameplay/MainHUD/Widget/AlertModal/UIC_HUDAlertModalWidget.h"
#include "ProjectISG/Core/UI/Gameplay/MainHUD/Widget/AlertModal/UIV_HUDAlertModalWidget.h"
#include "ProjectISG/Core/UI/HUD/Inventory/InventoryList.h"
#include "ProjectISG/Core/UI/HUD/Interactive/InteractiveUI.h"
#include "ProjectISG/Core/UI/HUD/Notify/InventoryNoticeWidget.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"
#include "ProjectISG/Systems/QuestStory/Component/QuestManageComponent.h"
#include "ProjectISG/Systems/QuestStory/Manager/QuestStoryManager.h"

void UUIC_MainHUD::AppearUI()
{
	Super::AppearUI();

	const UUIV_MainHUD* MainHUDView = Cast<UUIV_MainHUD>(GetView());
	const AMainPlayerCharacter* Player = GetView()->GetOwningPlayerPawn<
		AMainPlayerCharacter>();

	MainHUDView->GetMainSlotList()->UpdateItemData();
	const uint8 CurrentSlot = Player->GetPlayerInventoryComponent()->
	                                  GetCurrentSlotIndex();
	MainHUDView->GetMainSlotList()->SelectSlot(CurrentSlot, CurrentSlot);

	const AMainPlayerState* PS = Player->GetPlayerState<AMainPlayerState>();

	if (PS->GetInventoryComponent()->OnInventoryAddNotified.IsAlreadyBound(this, &ThisClass::NotifyFromInventory))
	{
		return;
	}
	PS->GetInventoryComponent()->OnInventoryAddNotified.AddDynamic(this, &ThisClass::NotifyFromInventory);
}

void UUIC_MainHUD::DisappearUI()
{
	Super::DisappearUI();

	const UUIV_MainHUD* MainHUDView = Cast<UUIV_MainHUD>(GetView());

	Cast<UUIC_AutoQuestDialogueWidget>(
			MainHUDView->GetAutoQuestDialogueWidget()->GetController())->
		SkipQuestDialogue();
}

void UUIC_MainHUD::UpdateMainHotSlot() const
{
	const UUIV_MainHUD* MainHUDView = Cast<UUIV_MainHUD>(GetView());
	MainHUDView->GetMainSlotList()->UpdateItemData();
}

void UUIC_MainHUD::SelectSlot(const uint8 CurrentSlot
                              , const uint8 NextSlot) const
{
	const UUIV_MainHUD* MainHUDView = Cast<UUIV_MainHUD>(GetView());
	MainHUDView->GetMainSlotList()->SelectSlot(CurrentSlot, NextSlot);
}

void UUIC_MainHUD::ToggleInteractiveUI(const bool IsShow)
{
	const UUIV_MainHUD* MainHUDView = Cast<UUIV_MainHUD>(GetView());

	MainHUDView->GetInteractiveUI()->ClearItems();

	MainHUDView->GetInteractiveUI()->SetInteractive(IsShow);
}

void UUIC_MainHUD::ToggleInteractiveUI(const FString& DisplayKey
                                       , const FString& DisplayText)
{
	const UUIV_MainHUD* MainHUDView = Cast<UUIV_MainHUD>(GetView());

	MainHUDView->GetInteractiveUI()->SetInteractive(true);
	MainHUDView->GetInteractiveUI()->SetInteractive(DisplayKey, DisplayText);
}

void UUIC_MainHUD::AdditiveToggleInteractiveUI(const FString& DisplayKey
                                               , const FString& DisplayText)
{
	const UUIV_MainHUD* MainHUDView = Cast<UUIV_MainHUD>(GetView());

	if (MainHUDView->GetInteractiveUI()->IsInteractiveHidden())
	{
		return;
	}

	MainHUDView->GetInteractiveUI()->AddInteractive(DisplayKey, DisplayText);
}

void UUIC_MainHUD::TogglePlacementIndicatorUI(const bool Visible)
{
	const UUIV_MainHUD* MainHUDView = Cast<UUIV_MainHUD>(GetView());

	MainHUDView->GetInteractiveUI()->SetPlacementIndicator(Visible);
}

void UUIC_MainHUD::ToggleAutoQuestUI(const bool IsActive)
{
	const UUIV_MainHUD* MainHUDView = Cast<UUIV_MainHUD>(GetView());

	MainHUDView->GetAutoQuestDialogueWidget()->SetVisibility(
		IsActive
			? ESlateVisibility::SelfHitTestInvisible
			: ESlateVisibility::Hidden);

	if (IsActive)
	{
		UUIC_AutoQuestDialogueWidget* AutoQuestDialogueWidgetController =
			Cast<UUIC_AutoQuestDialogueWidget>(
				MainHUDView->GetAutoQuestDialogueWidget()->GetController());

		AutoQuestDialogueWidgetController->StartQuestDialogue();
	}
}

void UUIC_MainHUD::ToggleCurrentQuestUI(const bool IsActive)
{
	const UUIV_MainHUD* MainHUDView = Cast<UUIV_MainHUD>(GetView());
	UUIC_CurrentQuestWidget* CurrentQuestWidgetController = Cast<
		UUIC_CurrentQuestWidget>(
		MainHUDView->GetCurrentQuestWidget()->GetController());

	if (IsActive)
	{
		const FString CurrentQuestId = GetView()->GetOwningPlayer<
			                                          AMainPlayerController>()->
		                                          GetQuestManageComponent()->
		                                          GetCurrentPlayingQuestId();

		const FQuestStoryData QuestData = UQuestStoryManager::GetQuestDataById(
			CurrentQuestId);

		MainHUDView->GetCurrentQuestWidget()->GetQuestTitle()->SetText(
			FText::FromString(QuestData.GetQuestTitle()));
		MainHUDView->GetCurrentQuestWidget()->GetQuestHint()->SetText(
			FText::FromString(QuestData.GetQuestHint()));

		CurrentQuestWidgetController->StartCurrentQuestAnimation();
	}
	else
	{
		MainHUDView->GetCurrentQuestWidget()->SetVisibility(
			ESlateVisibility::Hidden);
	}
}

void UUIC_MainHUD::AlertToMainHUD(const EAlertType AlertType
                                  , const FString& Message, const float Time)
{
	const UUIV_MainHUD* MainHUDView = Cast<UUIV_MainHUD>(GetView());
	UUIC_HUDAlertModalWidget* AlertModalWidgetController = Cast<
		UUIC_HUDAlertModalWidget>(
		MainHUDView->GetAlertModalWidget()->GetController());

	AlertModalWidgetController->Alert(AlertType, Message, Time);
}

void UUIC_MainHUD::NotifyFromInventory(const uint16 ItemId)
{
	const UUIV_MainHUD* MainHUDView = Cast<UUIV_MainHUD>(GetView());
	UInventoryNoticeWidget* NoticeWidget = MainHUDView->GetInventoryNoticeWidget();

	FItemInfoData ItemInfoData = UItemManager::GetItemInfoById(ItemId);
	
	NoticeWidget->OnNotify(ItemInfoData.GetThumbnail(), ItemInfoData.GetDisplayName());
}
