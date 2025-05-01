// Fill out your copyright notice in the Description page of Project Settings.


#include "UIC_ProductListWidget.h"

#include "UIV_ProductListWidget.h"
#include "Components/GridPanel.h"
#include "ProjectISG/Contents/Trading/TradingManager.h"
#include "ProjectISG/Core/UI/Popup/Trading/UI/ProductInfo/UIC_ProductInfoWidget.h"
#include "ProjectISG/Core/UI/Popup/Trading/UI/ProductInfo/UIV_ProductInfoWidget.h"

void UUIC_ProductListWidget::InitializeData()
{
	UUIV_ProductListWidget* ProductListWidgetView = Cast<
		UUIV_ProductListWidget>(GetView());

	ProductListWidgetView->GetProductListGrid()->ClearChildren();

	for (int i = 0; i < UTradingManager::GetProductData().Num(); i++)
	{
		UUIV_ProductInfoWidget* NewWidget = CreateWidget<
			UUIV_ProductInfoWidget>(ProductListWidgetView,
			                        ProductListWidgetView->
			                        GetProductInfoWidgetClass());

		ProductListWidgetView->GetProductListGrid()->AddChildToGrid(
			NewWidget, i / ProductListWidgetView->GetColumnValue(),
			i % ProductListWidgetView->GetColumnValue());
		Cast<UUIC_ProductInfoWidget>(NewWidget->GetController())->
			SetProductInfo(UTradingManager::GetProductData()[i].GetProductId());
	}
}
