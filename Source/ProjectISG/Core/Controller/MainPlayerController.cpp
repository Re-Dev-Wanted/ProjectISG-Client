#include "MainPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "ProjectISG/Core/UI/HUD/MainHUD.h"
#include "ProjectISG/Core/UI/HUD/Inventory/InventoryUI.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();
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
		return;
	}

	InventoryUI->SetVisibility(ESlateVisibility::Hidden);
	MainHUD->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	SetShowMouseCursor(false);
}

void AMainPlayerController::InitializeHUD()
{
	if (!IsLocalController())
	{
		return;
	}

	MainHUD = CreateWidget<UMainHUD>(this, MainHUDClass);
	MainHUD->AddToViewport();
}
