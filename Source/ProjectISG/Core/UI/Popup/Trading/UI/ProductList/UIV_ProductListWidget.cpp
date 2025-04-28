// Fill out your copyright notice in the Description page of Project Settings.


#include "UIV_ProductListWidget.h"

#include "UIC_ProductListWidget.h"
#include "ProjectISG/Core/UI/Popup/Trading/UI/ProductInfo/UIV_ProductInfoWidget.h"
#include "Components/GridPanel.h"

void UUIV_ProductListWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (ProductInfoWidgetClass)
	{
		for (int i = 0; i < DebugProductCount; i++)
		{
			UUIV_ProductInfoWidget* ProductInfoWidget = CreateWidget<
				UUIV_ProductInfoWidget>(this, ProductInfoWidgetClass);

			ProductListGrid->AddChildToGrid(ProductInfoWidget, i / ColumnValue,
			                                i % ColumnValue);
		}
	}
}

void UUIV_ProductListWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UUIC_ProductListWidget* ProductListWidgetController = Cast<
		UUIC_ProductListWidget>(GetController());

	ProductListWidgetController->InitializeData();
}
