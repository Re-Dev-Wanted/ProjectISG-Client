// Fill out your copyright notice in the Description page of Project Settings.


#include "UIC_ProductSellNotification.h"

#include "UIV_ProductSellNotification.h"
#include "Components/Button.h"
#include "ProjectISG/Contents/Trading/TradingManager.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/PlayerInventoryComponent.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Core/UI/Base/Components/UIManageComponent.h"
#include "ProjectISG/Core/UI/Popup/Inventory/UI/UIV_InventoryUI.h"
#include "ProjectISG/Core/UI/Popup/Trading/UI/UIC_TradingUI.h"
#include "ProjectISG/Core/UI/Popup/Trading/UI/UIM_TradingUI.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"
#include "ProjectISG/Systems/Logging/LoggingEnum.h"
#include "ProjectISG/Systems/Logging/LoggingStruct.h"
#include "ProjectISG/Systems/Logging/LoggingSubSystem.h"

void UUIC_ProductSellNotification::InitializeController(UBaseUIView* NewView,
	UBaseUIModel* NewModel)
{
	Super::InitializeController(NewView, NewModel);

	UUIV_ProductSellNotification* ProductSellNotification = Cast<
		UUIV_ProductSellNotification>(GetView());

	ProductSellNotification->GetSellButton()->OnClicked.AddDynamic(
		this, &ThisClass::OnClickedSellButton);
}

void UUIC_ProductSellNotification::OnClickedSellButton()
{
	UE_LOG(LogTemp, Warning, TEXT("아이템 팔기"))

	AMainPlayerController* PC = Cast<AMainPlayerController>(
		GetPlayerController());
	PC->PopUI();

	UUIC_TradingUI* TradingUIController = Cast<UUIC_TradingUI>(
		PC->GetUIManageComponent()->ControllerInstances[
			EUIName::Popup_TradingUI]);
	UUIM_TradingUI* TradingUIModel = Cast<UUIM_TradingUI>(
		TradingUIController->GetModel());

	AMainPlayerState* PS = GetPlayerController()->GetPlayerState<
		AMainPlayerState>();
	PS->GetInventoryComponent()->RemoveItem(
		TradingUIModel->GetClickedInventoryItem(), 1);

	AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		GetPlayerController()->GetPawn());
	Player->GetPlayerInventoryComponent()->UpdateInventorySlotItemData();

	PS->SetGold(PS->GetGold() + FindItemPrice(TradingUIModel));
	TradingUIController->UpdateGoldText();
	LoggingToSellItem();
}

int32 UUIC_ProductSellNotification::FindItemPrice(
	class UUIM_TradingUI* TradingUIModel)
{
	// 상품 데이터 테이블에 없으면 0원 처리 한다.

	for (int i = 0; i < UTradingManager::GetProductData().Num(); i++)
	{
		if (UTradingManager::GetProductData()[i].GetProductId() ==
			TradingUIModel->GetClickedInventoryItem())
		{
			return UTradingManager::GetProductData()[i].GetProductPrice();
		}
	}

	return 0;
}

void UUIC_ProductSellNotification::LoggingToSellItem()
{
	FDiaryLogParams LogParams;
	LogParams.Location = "거래장";
	LogParams.ActionType = ELoggingActionType::TRADE;
	LogParams.ActionName = ELoggingActionName::sell_item;

	GetWorld()->GetGameInstance()->GetSubsystem<ULoggingSubSystem>()->
	            LoggingData(LogParams);
}
