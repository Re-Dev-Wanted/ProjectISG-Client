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
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/PlayerInventoryComponent.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Core/UI/Base/Module/UI_BaseButton.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"

class AMainPlayerState;
class AMainPlayerCharacter;

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
	->OnClicked.AddUniqueDynamic(this, &UUIC_TradingUI::ChangeBuyState);

	TradingUIView->GetItemListView()->GetSellTabButton()->GetButton()
	->OnClicked.AddUniqueDynamic(this, &UUIC_TradingUI::ChangeSellState);

	TradingUIView->GetTradeButton()->Get()->OnClicked.AddUniqueDynamic
	(this, &UUIC_TradingUI::OnTrade);

	TradingUIView->GetTradeButton()->Get()->SetIsEnabled(false);

	UUIM_TradingUI* TradingUIModel = Cast<UUIM_TradingUI>(GetModel());
	TradingUIModel->SetCurrentState(ETradingState::BUY);

	TradingUIView->GetItemListView()->SetUpdateUI(TradingUIModel->GetCurrentState());

	UpdateGoldText();
}

void UUIC_TradingUI::OnCloseTradingUI()
{
	AMainPlayerController* PC = Cast<AMainPlayerController>(
		GetPlayerController());

	UUIV_TradingUI* TradingUIView = Cast<UUIV_TradingUI>(GetView());
	TradingUIView->GetProductDetailView()->OnHide();
	
	PC->PopUI();
}

void UUIC_TradingUI::OnTrade()
{
	
}

void UUIC_TradingUI::UpdateGoldText()
{
	AMainPlayerController* PC = Cast<AMainPlayerController>(
		GetPlayerController());
	AMainPlayerState* PS = PC->GetPlayerState<AMainPlayerState>();
	UUIV_TradingUI* TradingUIView = Cast<UUIV_TradingUI>(GetView());

	if (PC && PS && TradingUIView)
	{
		FString Str = FString::FromInt(PS->GetGold());
		TradingUIView->GetOwnedGoldText()->SetText(FText::FromString(Str));
	}
}

void UUIC_TradingUI::UpdateInventory()
{
	AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		GetPlayerController()->GetPawn());
	if (Player && Player->GetPlayerInventoryComponent())
	{
		Player->GetPlayerInventoryComponent()->UpdateInventorySlotItemData();
	}
}

void UUIC_TradingUI::OnUpdateSelectedProduct(uint16 ProductId)
{
	FProductStruct ProductStruct = UTradingManager::GetProductDataById(ProductId);
	FItemInfoData ItemInfoData = UItemManager::GetItemInfoById(ProductId);
	
	const UUIV_TradingUI* TradingUIView = Cast<UUIV_TradingUI>(GetView());
	const UUIM_TradingUI* TradingUIModel = Cast<UUIM_TradingUI>(GetModel());

	uint32 ProductPrice = TradingUIModel->GetCurrentState() == ETradingState::BUY?
		ProductStruct.GetProductPrice() * ProductStruct.GetBuyPriceRatio() :
		ProductStruct.GetProductPrice() * ProductStruct.GetSellPriceRatio();

	TradingUIView->GetProductDetailView()->UpdateUI(ItemInfoData
	.GetDisplayName(), ItemInfoData.GetDescription(), ProductPrice,
	ItemInfoData.GetThumbnail());

	AMainPlayerState* PS = GetPlayerController()->GetPlayerState<AMainPlayerState>();

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
	
	TradingUIView->GetItemListView()->SetUpdateUI(TradingUIModel->GetCurrentState());

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
	
	TradingUIView->GetItemListView()->SetUpdateUI(TradingUIModel->GetCurrentState());

	TradingUIView->GetProductDetailView()->OnHide();

	TradingUIView->GetTradeButton()->Get()->SetIsEnabled(false);
	TradingUIView->GetTradeButton()->GetText()->SetText(FText::FromString(TEXT
	("판매하기")));
}
