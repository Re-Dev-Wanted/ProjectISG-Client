// Fill out your copyright notice in the Description page of Project Settings.


#include "UIV_ProductInfoWidget.h"

#include "ProductItemWidgetObject.h"
#include "UIC_ProductInfoWidget.h"

void UUIV_ProductInfoWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	const UProductItemWidgetObject* ItemWidgetObject = 
	Cast<UProductItemWidgetObject>(ListItemObject);

	if (!ItemWidgetObject)
	{
		return;
	}

	if (UUIC_ProductInfoWidget* Widget = Cast<UUIC_ProductInfoWidget>(GetController
	()))
	{
		Widget->SetProductInfo(ItemWidgetObject->GetProductId(), 
		ItemWidgetObject->GetPriceRatio());
	}
}
