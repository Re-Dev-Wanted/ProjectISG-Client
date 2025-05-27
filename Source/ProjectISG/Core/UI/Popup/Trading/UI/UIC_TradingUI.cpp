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
	TradingUIView->SetOpenFlag(false);

	TradingUIView->GetProductDetailView()->OnHide();
	
	PC->PopUI();
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

void UUIC_TradingUI::SetItemInfoData(const uint8 InventoryIndex)
{
	const UUIV_TradingUI* TradingUIView = Cast<UUIV_TradingUI>(GetView());
	AMainPlayerState* PS = TradingUIView->GetOwningPlayerState<
		AMainPlayerState>();

	const FItemMetaInfo ItemMetaInfo = PS->GetInventoryComponent()->
	                                       GetInventoryList()[InventoryIndex];
}

void UUIC_TradingUI::DetectDragItem(uint16 ItemId, uint16 SlotIndex)
{
	UUIM_TradingUI* TradingModel = Cast<UUIM_TradingUI>(GetModel());
	TradingModel->SetClickedInventoryItem(ItemId);
	TradingModel->SetClickedInventorySlotIndex(SlotIndex);
}

void UUIC_TradingUI::OnUpdateSelectedProduct(uint16 ProductId)
{
	UE_LOG(LogTemp, Warning, TEXT("%d"), ProductId);

	FProductStruct ProductStruct = UTradingManager::GetProductDataById(ProductId);

	FItemInfoData ItemInfoData = UItemManager::GetItemInfoById(ProductId);
	
	const UUIV_TradingUI* TradingUIView = Cast<UUIV_TradingUI>(GetView());

	TradingUIView->GetProductDetailView()->UpdateUI(ItemInfoData
	.GetDisplayName(), ItemInfoData.GetDescription(), ProductStruct.GetProductPrice(),
	ItemInfoData.GetThumbnail());
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
}
