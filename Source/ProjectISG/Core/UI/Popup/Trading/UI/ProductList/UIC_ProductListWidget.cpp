// Fill out your copyright notice in the Description page of Project Settings.


#include "UIC_ProductListWidget.h"

#include "UIV_ProductListWidget.h"
#include "Components/ListView.h"
#include "ProjectISG/Contents/Trading/TradingManager.h"
#include "ProjectISG/Core/UI/Popup/Trading/UI/ProductInfo/ProductItemWidgetObject.h"
#include "ProjectISG/Core/UI/Popup/Trading/UI/ProductInfo/UIC_ProductInfoWidget.h"
#include "ProjectISG/Core/UI/Popup/Trading/UI/ProductInfo/UIV_ProductInfoWidget.h"
#include "ProjectISG/Systems/Inventory/ItemData.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"

void UUIC_ProductListWidget::InitializeData()
{
	UUIV_ProductListWidget* ProductListWidgetView = Cast<
		UUIV_ProductListWidget>(GetView());

	ProductListWidgetView->GetProductList()->ClearListItems();

	const TArray<FProductStruct> ProductDatas = UTradingManager::GetProductData();

	for (int i = 0, Count = ProductDatas.Num(); i < Count; i++)
	{
		const FProductStruct Product = ProductDatas[i];
		const FItemInfoData ItemInfoData = UItemManager::GetItemInfoById(Product.GetProductId());
		
		UProductItemWidgetObject* NewWidget = 
		NewObject<UProductItemWidgetObject>();

		NewWidget->ProductId = Product.GetProductId();

		ProductListWidgetView->GetProductList()->AddItem(NewWidget);
	}
}
