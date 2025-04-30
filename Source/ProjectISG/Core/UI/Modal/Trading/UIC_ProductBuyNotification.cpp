// Fill out your copyright notice in the Description page of Project Settings.


#include "UIC_ProductBuyNotification.h"

#include "UIV_ProductBuyNotification.h"
#include "Components/Button.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Core/UI/Popup/Trading/UI/UIV_TradingUI.h"
#include "ProjectISG/Core/UI/Popup/Trading/UI/ProductInfo/UIC_ProductInfoWidget.h"
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
		AMainPlayerState* PS = Player->GetPlayerState<AMainPlayerState>();
		
		PS->GetInventoryComponent()->AddItem(
			UItemManager::GetInitialItemMetaDataById(PC->GetClickedProductId()));

		PC->PopUI();
	}
}
