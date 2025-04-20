#include "MainPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Core/UI/HUD/MainHUD.h"
#include "ProjectISG/Core/UI/HUD/Inventory/InventoryList.h"
#include "ProjectISG/Core/UI/HUD/Inventory/InventoryUI.h"
#include "ProjectISG/Core/UI/HUD/Inventory/Module/ItemInfo.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//ConsoleCommand(TEXT("showdebug abilitysystem"));
	if (IsLocalController())
	{
		ItemInfoWidget = CreateWidget<UItemInfo>(this, ItemInfoWidgetClass);
		ItemInfoWidget->RemoveItemWidget();
		ItemInfoWidget->AddToViewport(9999);
	}
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

void AMainPlayerController::ShowItemInfo(const uint16 InventoryIndex) const
{
	const FItemMetaInfo ItemMetaInfo = GetPlayerState<AMainPlayerState>()->
	                                   GetInventoryComponent()->
	                                   GetInventoryList()[
		InventoryIndex];

	if (ItemMetaInfo.GetId() == 0)
	{
		return;
	}

	const FItemInfoData ItemInfoData = UItemManager::GetItemInfoById(
		ItemMetaInfo.GetId());

	ItemInfoWidget->ShowItemData(ItemMetaInfo, ItemInfoData);
}

void AMainPlayerController::RemoveItemInfo() const
{
	ItemInfoWidget->RemoveItemWidget();
}
