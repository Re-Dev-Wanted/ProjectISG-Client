#include "MainPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "ProjectISG/Core/UI/HUD/Inventory/InventoryUI.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AMainPlayerController::ToggleInventoryUI(const bool IsShow)
{
	if (!InventoryUI)
	{
		InventoryUI = CreateWidget<UInventoryUI>(this, InventoryUIClass);
		InventoryUI->AddToViewport();
	}

	if (IsShow)
	{
		InventoryUI->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		SetShowMouseCursor(true);
		return;
	}

	InventoryUI->SetVisibility(ESlateVisibility::Hidden);
	SetShowMouseCursor(false);
}
