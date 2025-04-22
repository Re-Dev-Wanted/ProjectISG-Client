#include "MainPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Core/UI/UIEnum.h"
#include "ProjectISG/Core/UI/Base/RootHUD.h"
#include "ProjectISG/Core/UI/Base/UIManageComponent.h"
#include "ProjectISG/Core/UI/HUD/MainHUD.h"
// #include "ProjectISG/Core/UI/HUD/Inventory/InventoryList.h"
#include "ProjectISG/Core/UI/HUD/Inventory/InventoryUI.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"

AMainPlayerController::AMainPlayerController()
{
	UIManageComponent = CreateDefaultSubobject<UUIManageComponent>(
		"UI Manage Component");
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ConsoleCommand(TEXT("showdebug abilitysystem"));

	if (IsLocalController())
	{
		URootHUD* RootHUDInstance = CreateWidget<URootHUD>(this, RootHUDClass);
		RootHUDInstance->AddToViewport();

		UIManageComponent->Initialize(RootHUDInstance);
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

	if (IsShow)
	{
		UIManageComponent->PushWidget(EUIName::Popup_InventoryUI);
	}
	else
	{
		UIManageComponent->PopWidget();
	}
}

TObjectPtr<UMainHUD> AMainPlayerController::GetMainHUD() const
{
	if (!UIManageComponent->WidgetInstances.FindRef(EUIName::Gameplay_MainHUD))
	{
		UIManageComponent->PushWidget(EUIName::Gameplay_MainHUD);
	}

	return Cast<UMainHUD>(
		UIManageComponent->WidgetInstances.FindRef(EUIName::Gameplay_MainHUD));
}

TObjectPtr<UInventoryUI> AMainPlayerController::GetInventoryUI() const
{
	if (!UIManageComponent->WidgetInstances.FindRef(EUIName::Popup_InventoryUI))
	{
		UIManageComponent->PushWidget(EUIName::Popup_InventoryUI);
	}

	return Cast<UInventoryUI>(
		UIManageComponent->WidgetInstances.FindRef(EUIName::Popup_InventoryUI));
}

void AMainPlayerController::PopUI()
{
	UIManageComponent->PopWidget();
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

	// ItemInfoWidget->ShowItemData(ItemMetaInfo, ItemInfoData);
}

void AMainPlayerController::RemoveItemInfo() const
{
	// ItemInfoWidget->RemoveItemWidget();
}
