// Fill out your copyright notice in the Description page of Project Settings.


#include "UIC_ProductBuyNotification.h"

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
		
		AMainPlayerState* PS = Player->GetPlayerState<AMainPlayerState>();
		PS->GetInventoryComponent()->AddItem(
			UItemManager::GetInitialItemMetaDataById(PC->GetClickedProductId()));

		Player->GetPlayerInventoryComponent()->UpdateInventorySlotItemData();

		int32 ItemPrice = FindItemPrice(PC);

		UUIC_TradingUI* TradingUIController = Cast<UUIC_TradingUI>(PC->GetUIManageComponent()->ControllerInstances[EUIName::Popup_TradingUI]);
		
		if (PS->GetGold() >= ItemPrice)
		{
			UE_LOG(LogTemp, Warning, TEXT("구매 전 골드 : %d"), PS->GetGold());
			PS->SetGold(PS->GetGold() - ItemPrice);
			TradingUIController->UpdateGoldText();
			UE_LOG(LogTemp, Warning, TEXT("구매 후 골드 : %d"), PS->GetGold());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("골드가 부족합니다."));
		}
	}
}

int32 UUIC_ProductBuyNotification::FindItemPrice(class AMainPlayerController* PC)
{
	for (int i = 0; i < UTradingManager::GetProductData().Num(); i++)
	{
		if (UTradingManager::GetProductData()[i].GetProductId() == PC->GetClickedProductId())
		{
			return UTradingManager::GetProductData()[i].GetProductPrice();
		}
	}

	return 0;
}
