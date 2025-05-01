// Fill out your copyright notice in the Description page of Project Settings.


#include "UIC_ProductSellNotification.h"

#include "UIV_ProductSellNotification.h"
#include "Components/Button.h"
#include "ProjectISG/Contents/Trading/TradingManager.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/PlayerInventoryComponent.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Core/UI/Popup/Inventory/UI/UIV_InventoryUI.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"

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

	AMainPlayerState* PS = GetPlayerController()->GetPlayerState<
		AMainPlayerState>();
	PS->GetInventoryComponent()->RemoveItem(PC->GetClickedInventoryItem(), 1);

	AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		GetPlayerController()->GetPawn());
	Player->GetPlayerInventoryComponent()->UpdateInventorySlotItemData();
	
	UE_LOG(LogTemp, Warning, TEXT("판매 전 골드 : %d"), PS->GetGold());
	PS->SetGold(PS->GetGold() + FindItemPrice(PC));
	UE_LOG(LogTemp, Warning, TEXT("판매 후 골드 : %d"), PS->GetGold());
}

int32 UUIC_ProductSellNotification::FindItemPrice(class AMainPlayerController* PC)
{
	for (int i = 0; i < UTradingManager::GetProductData().Num(); i++)
	{
		if (UTradingManager::GetProductData()[i].GetProductId() == PC->GetClickedInventoryItem())
		{
			return UTradingManager::GetProductData()[i].GetProductPrice();
		}
	}

	return 0;
}
