#include "UIC_MainHUD.h"

#include "UIM_MainHUD.h"
#include "UIV_MainHUD.h"
#include "Components/TextBlock.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/PlayerInventoryComponent.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/UI/Gameplay/QuestStory/Widget/AutoQuest/UIC_AutoQuestDialogueWidget.h"
#include "ProjectISG/Core/UI/Gameplay/QuestStory/Widget/AutoQuest/UIV_AutoQuestDialogueWidget.h"
#include "ProjectISG/Core/UI/Gameplay/QuestStory/Widget/CurrentQuest/UIC_CurrentQuestWidget.h"
#include "ProjectISG/Core/UI/Gameplay/QuestStory/Widget/CurrentQuest/UIV_CurrentQuestWidget.h"
#include "ProjectISG/Core/UI/HUD/Inventory/InventoryList.h"
#include "ProjectISG/Core/UI/HUD/Interactive/InteractiveUI.h"
#include "ProjectISG/Systems/QuestStory/Component/QuestManageComponent.h"
#include "ProjectISG/Systems/QuestStory/Manager/QuestStoryManager.h"

void UUIC_MainHUD::AppearUI()
{
	Super::AppearUI();

	const UUIV_MainHUD* MainHUDView = Cast<UUIV_MainHUD>(GetView());
	MainHUDView->GetMainSlotList()->UpdateItemData();

	const uint8 CurrentSlot = GetView()->GetOwningPlayerPawn<
		                                     AMainPlayerCharacter>()->
	                                     GetPlayerInventoryComponent()->
	                                     GetCurrentSlotIndex();

	MainHUDView->GetMainSlotList()->SelectSlot(CurrentSlot, CurrentSlot);
}

void UUIC_MainHUD::UpdateMainHotSlot() const
{
	const UUIV_MainHUD* MainHUDView = Cast<UUIV_MainHUD>(GetView());
	MainHUDView->GetMainSlotList()->UpdateItemData();
}

void UUIC_MainHUD::SelectSlot(const uint8 CurrentSlot,
                              const uint8 NextSlot) const
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

void UUIC_MainHUD::ToggleInteractiveUI(const FString& DisplayKey,
                                       const FString& DisplayText)
{
	const UUIV_MainHUD* MainHUDView = Cast<UUIV_MainHUD>(GetView());

	MainHUDView->GetInteractiveUI()->SetInteractive(true);
	MainHUDView->GetInteractiveUI()->SetInteractive(DisplayKey, DisplayText);
}

void UUIC_MainHUD::AdditiveToggleInteractiveUI(const FString& DisplayKey,
                                               const FString& DisplayText)
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

void UUIC_MainHUD::StartAutoQuest(const FString& QuestId)
{
	UUIM_MainHUD* MainHUDModel = Cast<UUIM_MainHUD>(GetModel());
	const UUIV_MainHUD* MainHUDView = Cast<UUIV_MainHUD>(GetView());

	MainHUDModel->SetCurrentPlayingAutoQuestId(QuestId);
	ToggleAutoQuestUI(true);

	Cast<UUIC_AutoQuestDialogueWidget>(
			MainHUDView->GetAutoQuestDialogueWidget()->GetController())->
		StartQuestDialogue();

	auto a = MainHUDView->GetCurrentQuestWidget();
}

void UUIC_MainHUD::ToggleAutoQuestUI(const bool IsActive)
{
	const UUIV_MainHUD* MainHUDView = Cast<UUIV_MainHUD>(GetView());
	MainHUDView->GetAutoQuestDialogueWidget()->SetVisibility(
		IsActive
			? ESlateVisibility::SelfHitTestInvisible
			: ESlateVisibility::Hidden);
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
		MainHUDView->GetCurrentQuestWidget()->GetQuestDescription()->SetText(
			FText::FromString(QuestData.GetQuestDescription()));

		CurrentQuestWidgetController->StartCurrentQuestAnimation();
	}
	else
	{
		MainHUDView->GetCurrentQuestWidget()->SetVisibility(
			ESlateVisibility::Hidden);
	}
}
