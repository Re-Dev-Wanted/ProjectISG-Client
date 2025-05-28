
// Fill out your copyright notice in the Description page of Project Settings.


#include "UIC_ProductInfoWidget.h"

#include "UIM_ProductInfoWidget.h"
#include "UIV_ProductInfoWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "ProjectISG/Contents/Trading/TradingManager.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/UI/Base/Components/UIManageComponent.h"
#include "ProjectISG/Core/UI/Modal/Trading/UIC_ProductBuyNotification.h"
#include "ProjectISG/Core/UI/Modal/Trading/UIM_ProductBuyNotification.h"
#include "ProjectISG/Core/UI/Popup/Trading/UI/ProductThumbnailWidget.h"
#include "ProjectISG/Core/UI/Popup/Trading/UI/UIC_TradingUI.h"
#include "ProjectISG/Systems/Inventory/ItemData.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"


void UUIC_ProductInfoWidget::InitializeController(UBaseUIView* NewView,
                                                  UBaseUIModel* NewModel)
{
	Super::InitializeController(NewView, NewModel);

	const UUIV_ProductInfoWidget* ProductInfoWidget = Cast<
		UUIV_ProductInfoWidget>(GetView());
	
	ProductInfoWidget->GetProductSelectButton()->OnClicked.AddDynamic(this, 
	&UUIC_ProductInfoWidget::OnSelectProductData);
}

void UUIC_ProductInfoWidget::SetProductInfo(const uint32 ProductId, const uint32 Count, const float Ratio)
{
	const UUIV_ProductInfoWidget* ProductInfoWidget = Cast<
		UUIV_ProductInfoWidget>(GetView());

	UUIM_ProductInfoWidget* ProductInfoModel = Cast<UUIM_ProductInfoWidget>(
		GetModel());
	ProductInfoModel->SetProductId(ProductId);

	const FItemInfoData ItemInfoData = UItemManager::GetItemInfoById(ProductId);

	UTexture2D* Thumbnail = ItemInfoData.GetThumbnail().LoadSynchronous();
	ProductInfoWidget->GetThumbnailWidget()->SetWidget(Thumbnail);

	ProductInfoWidget->GetThumbnailWidget()->SetCount(Count);
	
	ProductInfoWidget->GetProductName()->SetText(
		FText::FromString(ItemInfoData.GetDisplayName()));

	uint32 ProductPrice = UTradingManager::GetProductPriceById(ProductId) * Ratio;
	FString Str = FString::FromInt(ProductPrice);
	ProductInfoWidget->GetProductPrice()->SetText(FText::FromString(Str));
}

void UUIC_ProductInfoWidget::OnSelectProductData()
{
	AMainPlayerController* PC = Cast<AMainPlayerController>(GetPlayerController());
	if (PC)
	{
		uint32 ProductId = Cast<UUIM_ProductInfoWidget>(
		GetModel())->GetProductId();

		UUIC_TradingUI* TradingUI = Cast<UUIC_TradingUI>
		(PC->GetUIManageComponent()->ControllerInstances[EUIName::Popup_TradingUI]);

		TradingUI->OnUpdateSelectedProduct(ProductId);
	}
}
