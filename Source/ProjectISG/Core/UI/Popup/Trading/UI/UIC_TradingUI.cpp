#include "UIC_TradingUI.h"

#include "EnhancedInputComponent.h"
#include "TradingTabButton.h"
#include "UIM_TradingUI.h"
#include "UIV_TradingUI.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "ProductInfo/ProductDetailView.h"
#include "ProductList/UIV_ProductListWidget.h"
#include "ProjectISG/Contents/Trading/TradingManager.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Core/UI/Base/Components/UIManageComponent.h"
#include "ProjectISG/Core/UI/Base/Module/UI_BaseButton.h"
#include "ProjectISG/Core/UI/Modal/Trading/UIC_ProductBuyNotification.h"
#include "ProjectISG/Core/UI/Modal/Trading/UIM_ProductBuyNotification.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"
#include "ProjectISG/Systems/Logging/LoggingStruct.h"
#include "ProjectISG/Systems/Logging/LoggingSubSystem.h"

void UUIC_TradingUI::BindInputAction(UEnhancedInputComponent* InputComponent)
{
	Super::BindInputAction(InputComponent);

	InputComponent->BindAction(CloseTradingUI, ETriggerEvent::Triggered, this,
	                           &ThisClass::OnCloseTradingUI);
}

void UUIC_TradingUI::AppearUI()
{
	Super::AppearUI();

	UUIV_TradingUI* TradingUIView = Cast<UUIV_TradingUI>(GetView());
	TradingUIView->GetCloseButton()->OnClicked.AddUniqueDynamic(this,
		&ThisClass::OnCloseTradingUI);

	TradingUIView->GetItemListView()->GetBuyTabButton()->GetButton()
	             ->OnClicked.AddUniqueDynamic(
		             this, &UUIC_TradingUI::ChangeBuyState);

	TradingUIView->GetItemListView()->GetSellTabButton()->GetButton()
	             ->OnClicked.AddUniqueDynamic(
		             this, &UUIC_TradingUI::ChangeSellState);

	TradingUIView->GetTradeButton()->Get()->OnClicked.AddUniqueDynamic
		(this, &UUIC_TradingUI::OnTrade);

	RefreshList();

	UpdateGoldText();
}

void UUIC_TradingUI::OnCloseTradingUI()
{
	AMainPlayerController* PC = Cast<AMainPlayerController>(
		GetPlayerController());

	UUIV_TradingUI* TradingUIView = Cast<UUIV_TradingUI>(GetView());
	TradingUIView->GetProductDetailView()->OnHide();

	TradingUIView->GetTradeButton()->Get()->SetIsEnabled(false);

	UUIM_TradingUI* TradingUIModel = Cast<UUIM_TradingUI>(GetModel());
	TradingUIModel->SetCurrentState(ETradingState::BUY);

	ResetUIFromPlayerController();
}

void UUIC_TradingUI::OnTrade()
{
	AMainPlayerController* PC = Cast<AMainPlayerController>(
		GetPlayerController());
	if (PC)
	{
		UUIM_TradingUI* TradingUIModel = Cast<UUIM_TradingUI>(GetModel());

		PC->PushUI(EUIName::Modal_BuyNotification);

		UUIC_ProductBuyNotification* ModalController = Cast<
			UUIC_ProductBuyNotification>(
			PC->GetUIManageComponent()->ControllerInstances[
				EUIName::Modal_BuyNotification]);
		UUIM_ProductBuyNotification* ModalModel = Cast<
			UUIM_ProductBuyNotification>(ModalController->GetModel());

		ModalModel->SetCount(1);
		ModalModel->SetClickedProductId(TradingUIModel->GetSelectedId());
		ModalModel->SetTradingState(TradingUIModel->GetCurrentState());

		ModalController->OnInitialize();
	}
}

void UUIC_TradingUI::RefreshList()
{
	const UUIV_TradingUI* TradingUIView = Cast<UUIV_TradingUI>(GetView());
	UUIM_TradingUI* TradingUIModel = Cast<UUIM_TradingUI>(GetModel());
	FProductStruct ProductStruct = UTradingManager::GetProductDataById
		(TradingUIModel->GetSelectedId());

	AMainPlayerState* PS = GetPlayerController()->GetPlayerState<
		AMainPlayerState>();

	if (!PS)
	{
		return;
	}
	
	if (TradingUIModel->GetCurrentState() == ETradingState::SELL)
	{
		uint16 RemainCount = PS->GetInventoryComponent()->GetCurrentCount
			(TradingUIModel->GetSelectedId());

		if (RemainCount == 0)
		{
			TradingUIModel->SetSelectedId(0);
			TradingUIView->GetProductDetailView()->OnHide();
			TradingUIView->GetTradeButton()->Get()->SetIsEnabled(false);
		}
	}
	else
	{
		const uint32 ProductPrice = ProductStruct.GetProductPrice() * ProductStruct.GetBuyPriceRatio();
		
		TradingUIView->GetTradeButton()->Get()->SetIsEnabled(TradingUIModel->GetSelectedId() > 0 && PS->GetGold() >= ProductPrice);
	}
	
	TradingUIView->GetItemListView()->SetUpdateUI(
		TradingUIModel->GetCurrentState());
}

void UUIC_TradingUI::UpdateGoldText()
{
	AMainPlayerController* PC = Cast<AMainPlayerController>(
		GetPlayerController());
	AMainPlayerState* PS = PC->GetPlayerState<AMainPlayerState>();
	UUIV_TradingUI* TradingUIView = Cast<UUIV_TradingUI>(GetView());

	if (PC && PS && TradingUIView)
	{
		TradingUIView->GetOwnedGoldText()->SetText(FText::AsNumber(PS->GetGold()));
	}
}

void UUIC_TradingUI::OnUpdateSelectedProduct(uint16 ProductId)
{
	FProductStruct ProductStruct = UTradingManager::GetProductDataById(
		ProductId);
	FItemInfoData ItemInfoData = UItemManager::GetItemInfoById(ProductId);

	const UUIV_TradingUI* TradingUIView = Cast<UUIV_TradingUI>(GetView());
	UUIM_TradingUI* TradingUIModel = Cast<UUIM_TradingUI>(GetModel());

	TradingUIModel->SetSelectedId(ProductId);

	const uint32 ProductPrice = TradingUIModel->GetCurrentState() ==
	                            ETradingState::BUY
		                            ? ProductStruct.GetProductPrice() *
		                            ProductStruct.GetBuyPriceRatio()
		                            : ProductStruct.GetProductPrice() *
		                            ProductStruct.GetSellPriceRatio();

	TradingUIView->GetProductDetailView()->UpdateUI(ItemInfoData
	                                                .GetDisplayName(),
	                                                ItemInfoData.
	                                                GetDescription(),
	                                                ProductPrice,
	                                                ItemInfoData.
	                                                GetThumbnail());

	AMainPlayerState* PS = GetPlayerController()->GetPlayerState<
		AMainPlayerState>();

	bool IsEnabled = TradingUIModel->GetCurrentState() == ETradingState::SELL ||
		PS->GetGold() >= ProductPrice;

	TradingUIView->GetTradeButton()->Get()->SetIsEnabled(IsEnabled);
}

void UUIC_TradingUI::ChangeBuyState()
{
	UUIM_TradingUI* TradingUIModel = Cast<UUIM_TradingUI>(GetModel());

	if (TradingUIModel->GetCurrentState() == ETradingState::BUY)
	{
		return;
	}

	TradingUIModel->SetCurrentState(ETradingState::BUY);

	UUIV_TradingUI* TradingUIView = Cast<UUIV_TradingUI>(GetView());

	TradingUIView->GetItemListView()->SetUpdateUI(
		TradingUIModel->GetCurrentState());

	TradingUIView->GetProductDetailView()->OnHide();

	TradingUIView->GetTradeButton()->Get()->SetIsEnabled(false);
	TradingUIView->GetTradeButton()->GetText()->SetText(FText::FromString(TEXT
		("구매하기")));
}

void UUIC_TradingUI::ChangeSellState()
{
	UUIM_TradingUI* TradingUIModel = Cast<UUIM_TradingUI>(GetModel());

	if (TradingUIModel->GetCurrentState() == ETradingState::SELL)
	{
		return;
	}

	TradingUIModel->SetCurrentState(ETradingState::SELL);

	UUIV_TradingUI* TradingUIView = Cast<UUIV_TradingUI>(GetView());

	TradingUIView->GetItemListView()->SetUpdateUI(
		TradingUIModel->GetCurrentState());

	TradingUIView->GetProductDetailView()->OnHide();

	TradingUIView->GetTradeButton()->Get()->SetIsEnabled(false);
	TradingUIView->GetTradeButton()->GetText()->SetText(FText::FromString(TEXT
		("판매하기")));
}
