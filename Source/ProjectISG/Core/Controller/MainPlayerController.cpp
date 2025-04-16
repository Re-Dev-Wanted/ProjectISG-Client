#include "MainPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "ProjectISG/Core/UI/HUD/Inventory/InventoryUI.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	InventoryUI = CreateWidget<UInventoryUI>(this, InventoryUIClass);
	InventoryUI->AddToViewport();
	SetShowMouseCursor(true);
}
