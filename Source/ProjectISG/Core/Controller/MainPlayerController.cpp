#include "MainPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Core/UI/HUD/MainHUD.h"
#include "ProjectISG/Core/UI/HUD/Inventory/InventoryList.h"
#include "ProjectISG/Core/UI/HUD/Inventory/InventoryUI.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AMainPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	GetPlayerState<AMainPlayerState>()->InitializeData();
}

void AMainPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (HasAuthority())
	{
		GetPlayerState<AMainPlayerState>()->InitializeData();
	}
}

void AMainPlayerController::ToggleInventoryUI(const bool IsShow)
{
	if (!IsLocalController())
	{
		return;
	}

	if (!InventoryUI)
	{
		InventoryUI = CreateWidget<UInventoryUI>(this, InventoryUIClass);
		InventoryUI->AddToViewport();
	}

	if (IsShow)
	{
		InventoryUI->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		MainHUD->SetVisibility(ESlateVisibility::Hidden);

		SetShowMouseCursor(true);
		DisableInput(this);

		return;
	}

	InventoryUI->SetVisibility(ESlateVisibility::Hidden);
	MainHUD->GetMainSlotList()->UpdateItemData();
	MainHUD->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	SetShowMouseCursor(false);
	EnableInput(this);
}

void AMainPlayerController::InitializeHUD()
{
	if (!IsLocalController())
	{
		return;
	}

	MainHUD = CreateWidget<UMainHUD>(this, MainHUDClass);
	MainHUD->AddToViewport();
	MainHUD->InitializeHUD();
}
