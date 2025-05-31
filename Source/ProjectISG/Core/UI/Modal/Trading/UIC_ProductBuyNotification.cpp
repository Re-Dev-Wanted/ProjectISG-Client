#include "UIC_ProductBuyNotification.h"

#include "UIM_ProductBuyNotification.h"
#include "UIV_ProductBuyNotification.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectISG/Contents/Trading/ProductStruct.h"
#include "ProjectISG/Contents/Trading/TradingManager.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
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

	const UUIV_ProductBuyNotification* ProductNotification = Cast<
		UUIV_ProductBuyNotification>(GetView());

	ProductNotification->GetButton()->OnClicked.AddUniqueDynamic(
		this, &ThisClass::OnClickedButton);

	ProductNotification->GetCloseButton()->OnClicked.AddUniqueDynamic(this, &ThisClass::PopUIFromPlayerController);
}

void UUIC_ProductBuyNotification::AppearUI()
{
	Super::AppearUI();
	
}

void UUIC_ProductBuyNotification::OnInitialize()
{
	UUIV_ProductBuyNotification* ProductNotification = Cast<
		UUIV_ProductBuyNotification>(GetView());

	ProductNotification->OnInitialize();
}

void UUIC_ProductBuyNotification::OnClickedButton()
{
	UGameplayStatics::PlaySound2D(GetWorld(), ButtonSFX, 1, 1, 0.25f);
	
	UUIM_ProductBuyNotification* UIModel = Cast<UUIM_ProductBuyNotification>(GetModel());
	FProductStruct ProductStruct = UTradingManager::GetProductDataById(UIModel->GetClickedProductId());

	AMainPlayerState* PS = GetPlayerController()->GetPlayerState<AMainPlayerState>();

	switch (UIModel->GetTradingState())
	{
		case ETradingState::SELL:
		{
			FItemMetaInfo MetaInfo = PS->GetInventoryComponent()->GetFirstMetaInfo
			(UIModel->GetClickedProductId());
	
			float PriceRatio = UItemManager::GetPriceRatio(MetaInfo);
				
			PS->GetInventoryComponent()->RemoveItem
			(UIModel->GetClickedProductId(), UIModel->GetCount());
	
			uint32 SellPrice = ProductStruct.GetProductPrice() * 
			ProductStruct.GetSellPriceRatio() * PriceRatio * UIModel->GetCount();
	
			PS->SetGold(PS->GetGold() + SellPrice);
		}
		break;
		default:
		{
			FItemMetaInfo MetaInfo = UItemManager::GetInitialItemMetaDataById(UIModel->GetClickedProductId());
			MetaInfo.SetCurrentCount(UIModel->GetCount());
			PS->GetInventoryComponent()->AddItem(MetaInfo);
		
			uint32 ItemPrice = ProductStruct.GetProductPrice() * ProductStruct.GetBuyPriceRatio() * UIModel->GetCount();
			PS->SetGold(PS->GetGold() - ItemPrice);
		}
		break;
	}

	Logging();

	PopUIFromPlayerController();
}

void UUIC_ProductBuyNotification::UpdateCount(int Count)
{
	UUIM_ProductBuyNotification* UIModel = Cast<UUIM_ProductBuyNotification>(GetModel());
	UIModel->SetCount(Count);
}

void UUIC_ProductBuyNotification::Logging()
{
	UUIM_ProductBuyNotification* UIModel = Cast<UUIM_ProductBuyNotification>(GetModel());

	const ETradingState State = UIModel->GetTradingState();
	const FItemInfoData ItemInfoData = UItemManager::GetItemInfoById(UIModel->GetClickedProductId());
	
	FDiaryLogParams LogParams;
	LogParams.Location = TEXT("거래장");
	LogParams.ActionType = ELoggingActionType::TRADE;
	LogParams.ActionName = State == ETradingState::BUY? ELoggingActionName::buy_item : ELoggingActionName::sell_item;
	FString Word = State == ETradingState::BUY? TEXT("구매") : TEXT("판매");
	LogParams.Detail = FString::Printf(TEXT("%s(을)를 %d개 %s했다."), *ItemInfoData.GetDisplayName(), UIModel->GetCount(), *Word);

	GetWorld()->GetGameInstance()->GetSubsystem<ULoggingSubSystem>()->
	            LoggingData(LogParams);
}
