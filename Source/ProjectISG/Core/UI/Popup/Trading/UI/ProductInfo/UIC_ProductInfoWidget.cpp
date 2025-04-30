// Fill out your copyright notice in the Description page of Project Settings.


#include "UIC_ProductInfoWidget.h"

#include "UIM_ProductInfoWidget.h"
#include "UIV_ProductInfoWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Systems/Inventory/ItemData.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"


void UUIC_ProductInfoWidget::InitializeController(UBaseUIView* NewView,
                                                  UBaseUIModel* NewModel)
{
	Super::InitializeController(NewView, NewModel);

	const UUIV_ProductInfoWidget* ProductInfoWidget = Cast<
		UUIV_ProductInfoWidget>(GetView());

	ProductInfoWidget->GetProductSelectButton()->OnClicked.AddDynamic(
		this, &ThisClass::OnSelectProductData);
}

void UUIC_ProductInfoWidget::SetProductInfo(const uint32 ProductId)
{
	const UUIV_ProductInfoWidget* ProductInfoWidget = Cast<
		UUIV_ProductInfoWidget>(GetView());

	UUIM_ProductInfoWidget* ProductInfoModel = Cast<UUIM_ProductInfoWidget>(
		GetModel());
	ProductInfoModel->SetProductId(ProductId);

	const FItemInfoData ItemInfoData = UItemManager::GetItemInfoById(ProductId);

	UTexture2D* Thumbnail = ItemInfoData.GetThumbnail().LoadSynchronous();
	ProductInfoWidget->GetProductThumbnail()->SetBrushFromTexture(Thumbnail);
	ProductInfoWidget->GetProductName()->SetText(
		FText::FromString(ItemInfoData.GetDisplayName()));
}

void UUIC_ProductInfoWidget::OnSelectProductData()
{
	// 선택 하면 구매 확인 widget을 생성해서 띄운다.
	UE_LOG(LogTemp, Warning, TEXT("구매 확인창 띄우기"));

	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		GetPlayerController()->GetPawn());
	if (Player)
	{
		uint32 ProductId = Cast<UUIM_ProductInfoWidget>(
		GetModel())->GetProductId();
		Player->GetController<AMainPlayerController>()->SetClickedProductId(ProductId); 
		Player->GetController<AMainPlayerController>()->PushUI(
			EUIName::Modal_BuyNotification);
	}
}
