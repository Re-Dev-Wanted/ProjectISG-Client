#include "MainPlayerController.h"

#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Core/UI/UIEnum.h"
#include "ProjectISG/Core/UI/Base/Components/UIManageComponent.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "ProjectISG/Core/UI/Gameplay/MainHUD/UI/UIC_MainHUD.h"
#include "ProjectISG/Core/UI/Popup/Inventory/UI/UIC_InventoryUI.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"

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
		UIManageComponent->Initialize();
		UIManageComponent->PushWidget(EUIName::Gameplay_MainHUD);
	}
}

void AMainPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (IsLocalController())
	{
		GetPlayerState<AMainPlayerState>()->InitializeData();
	}
}

void AMainPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (IsLocalController() && HasAuthority())
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
		if (UIManageComponent->GetLastStackUI() == EUIName::Popup_InventoryUI)
		{
			UIManageComponent->PopWidget();
		}
	}
}

TObjectPtr<UUIC_MainHUD> AMainPlayerController::GetMainHUD() const
{
	// 컨트롤러가 없는 경우에만 만들기
	if (!UIManageComponent->ControllerInstances.FindRef(
		EUIName::Gameplay_MainHUD))
	{
		UIManageComponent->PushWidget(EUIName::Gameplay_MainHUD);
	}

	return Cast<UUIC_MainHUD>(
		UIManageComponent->ControllerInstances.FindRef(
			EUIName::Gameplay_MainHUD));
}

TObjectPtr<UUIC_InventoryUI> AMainPlayerController::GetInventoryUI() const
{
	// 컨트롤러가 없는 경우에만 만들기
	if (!UIManageComponent->ControllerInstances.FindRef(
		EUIName::Popup_InventoryUI))
	{
		UIManageComponent->PushWidget(EUIName::Popup_InventoryUI);
	}

	return Cast<UUIC_InventoryUI>(
		UIManageComponent->ControllerInstances.FindRef(
			EUIName::Popup_InventoryUI));
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
	}

	// UIManageComponent->PushWidget(EUIName::Modal_ItemInfo);

	// UItemInfo* ItemInfoWidget = Cast<UItemInfo>(
	// 	UIManageComponent->WidgetInstances[EUIName::Modal_ItemInfo]);
	// ItemInfoWidget->ShowItemData(ItemMetaInfo);
}

void AMainPlayerController::RemoveItemInfo() const
{
	// if (UIManageComponent->GetLastStackUI() == EUIName::Modal_ItemInfo)
	// {
	// 	UIManageComponent->PopWidget();
	// }
}
