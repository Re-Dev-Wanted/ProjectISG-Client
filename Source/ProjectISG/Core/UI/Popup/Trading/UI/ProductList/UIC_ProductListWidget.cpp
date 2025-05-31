// Fill out your copyright notice in the Description page of Project Settings.


#include "UIC_ProductListWidget.h"

#include "UIV_ProductListWidget.h"
#include "Components/ListView.h"
#include "ProjectISG/Contents/Trading/TradingManager.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Core/UI/Popup/Trading/UI/UIM_TradingUI.h"
#include "ProjectISG/Core/UI/Popup/Trading/UI/ProductInfo/ProductItemWidgetObject.h"
#include "ProjectISG/Core/UI/Popup/Trading/UI/ProductInfo/UIC_ProductInfoWidget.h"
#include "ProjectISG/Core/UI/Popup/Trading/UI/ProductInfo/UIV_ProductInfoWidget.h"
#include "ProjectISG/Systems/Inventory/ItemData.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"

void UUIC_ProductListWidget::InitializeData()
{
	SetProductList();
}

void UUIC_ProductListWidget::OnUpdateList(ETradingState State)
{
	switch (State)
	{
	case ETradingState::SELL:
		SetInventoryList();
		break;
	default:
		SetProductList();
		break;
	}
}

void UUIC_ProductListWidget::SetProductList()
{
	UUIV_ProductListWidget* ProductListWidgetView = Cast<
		UUIV_ProductListWidget>(GetView());
	
	ProductListWidgetView->GetProductList()->ClearListItems();

	const TArray<FProductStruct> ProductDatas = UTradingManager::GetProductData();

	for (int i = 0, Count = ProductDatas.Num(); i < Count; i++)
	{
		const FProductStruct Product = ProductDatas[i];
		
		UProductItemWidgetObject* NewWidget = 
			NewObject<UProductItemWidgetObject>();

		NewWidget->SetProductId(Product.GetProductId());
		NewWidget->SetPriceRatio(Product.GetBuyPriceRatio());
		NewWidget->SetProductCount(0);

		ProductListWidgetView->GetProductList()->AddItem(NewWidget);
	}
}

void UUIC_ProductListWidget::SetInventoryList()
{
	UUIV_ProductListWidget* ProductListWidgetView = Cast<
		UUIV_ProductListWidget>(GetView());
	
	ProductListWidgetView->GetProductList()->ClearListItems();

	const AMainPlayerState* PS =GetPlayerController()
	->GetPlayerState<AMainPlayerState>();

	if (!PS)
	{
		return;
	}

	for (TPair Pair : PS->GetInventoryComponent()->GetCurrentRemainItemMetaValue())
	{
		FItemMetaInfo MetaInfo = Pair.Key;
		uint32 Count = Pair.Value;
		
		if (!MetaInfo.IsValid())
		{
			continue;
		}

		if (!UTradingManager::IsTradable(MetaInfo.GetId()))
		{
			continue;
		}

		FProductStruct ProductData = UTradingManager::GetProductDataById
		(MetaInfo.GetId());

		UProductItemWidgetObject* NewWidget = 
			NewObject<UProductItemWidgetObject>();

		NewWidget->SetProductId(MetaInfo.GetId());
		NewWidget->SetPriceRatio(ProductData.GetSellPriceRatio());
		NewWidget->SetProductCount(Count);

		ProductListWidgetView->GetProductList()->AddItem(NewWidget);
	}
}
