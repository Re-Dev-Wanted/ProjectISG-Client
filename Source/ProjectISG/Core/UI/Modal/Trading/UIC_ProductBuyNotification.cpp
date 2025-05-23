﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "UIC_ProductBuyNotification.h"

#include "UIM_ProductBuyNotification.h"
#include "UIV_ProductBuyNotification.h"
#include "Components/Button.h"
#include "ProjectISG/Contents/Trading/TradingManager.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/PlayerInventoryComponent.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Core/UI/Base/Components/UIManageComponent.h"
#include "ProjectISG/Core/UI/Popup/Trading/UI/UIC_TradingUI.h"
#include "ProjectISG/Core/UI/Popup/Trading/UI/UIV_TradingUI.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"
#include "ProjectISG/Systems/Logging/LoggingEnum.h"
#include "ProjectISG/Systems/Logging/LoggingStruct.h"
#include "ProjectISG/Systems/Logging/LoggingSubSystem.h"

void UUIC_ProductBuyNotification::InitializeController(UBaseUIView* NewView,
	UBaseUIModel* NewModel)
{
	Super::InitializeController(NewView, NewModel);

	const UUIV_ProductBuyNotification* ProductBuyNotification = Cast<
		UUIV_ProductBuyNotification>(GetView());

	ProductBuyNotification->GetBuyButton()->OnClicked.AddDynamic(
		this, &ThisClass::OnClickedBuyButton);
}

void UUIC_ProductBuyNotification::OnClickedBuyButton()
{
	UE_LOG(LogTemp, Warning, TEXT("구매하기"));

	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		GetPlayerController()->GetPawn());
	if (Player)
	{
		AMainPlayerController* PC = Player->GetController<
			AMainPlayerController>();
		PC->PopUI();

		UUIM_ProductBuyNotification* ProductBuyNotificationModel = Cast<
			UUIM_ProductBuyNotification>(GetModel());

		AMainPlayerState* PS = Player->GetPlayerState<AMainPlayerState>();
		PS->GetInventoryComponent()->AddItem(
			UItemManager::GetInitialItemMetaDataById(
				ProductBuyNotificationModel->GetClickedProductId()));

		Player->GetPlayerInventoryComponent()->UpdateInventorySlotItemData();

		int32 ItemPrice = FindItemPrice(ProductBuyNotificationModel);

		UUIC_TradingUI* TradingUIController = Cast<UUIC_TradingUI>(
			PC->GetUIManageComponent()->ControllerInstances[
				EUIName::Popup_TradingUI]);

		if (PS->GetGold() >= ItemPrice)
		{
			PS->SetGold(PS->GetGold() - ItemPrice);
			TradingUIController->UpdateGoldText();
			LoggingToBuyItem();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("골드가 부족합니다."));
		}
	}
}

int32 UUIC_ProductBuyNotification::FindItemPrice(
	class UUIM_ProductBuyNotification* ProductBuyNotificationModel)
{
	for (int i = 0; i < UTradingManager::GetProductData().Num(); i++)
	{
		if (UTradingManager::GetProductData()[i].GetProductId() ==
			ProductBuyNotificationModel->GetClickedProductId())
		{
			return UTradingManager::GetProductData()[i].GetProductPrice();
		}
	}

	return 0;
}

void UUIC_ProductBuyNotification::LoggingToBuyItem()
{
	FDiaryLogParams LogParams;
	LogParams.Location = TEXT("거래장");
	LogParams.ActionType = ELoggingActionType::TRADE;
	LogParams.ActionName = ELoggingActionName::buy_item;

	GetWorld()->GetGameInstance()->GetSubsystem<ULoggingSubSystem>()->
	            LoggingData(LogParams);
}
