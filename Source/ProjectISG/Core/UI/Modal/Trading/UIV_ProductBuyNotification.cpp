#include "UIV_ProductBuyNotification.h"

#include <ProjectISG/Core/UI/Base/Module/UI_EscapeButton.h>

#include "UIC_ProductBuyNotification.h"
#include "UIM_ProductBuyNotification.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "ProjectISG/Contents/Trading/ProductStruct.h"
#include "ProjectISG/Contents/Trading/TradingManager.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Systems/Inventory/ItemData.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"

class AMainPlayerState;

void UUIV_ProductBuyNotification::NativeConstruct()
{
	Super::NativeConstruct();

	Inc_Button->OnClicked.AddUniqueDynamic(this, &ThisClass::Increase);
	Dec_Button->OnClicked.AddUniqueDynamic(this, &ThisClass::Decrease);

	CountSlider->OnValueChanged.AddUniqueDynamic(this, &ThisClass::OnCountValueChanged);
}

void UUIV_ProductBuyNotification::OnInitialize()
{
	CountSlider->SetMinValue(1);
	CountText->SetText(FText::FromString(TEXT("x1")));
	
	UUIM_ProductBuyNotification* UIModel = Cast<UUIM_ProductBuyNotification>(GetModel());

	FProductStruct ProductStruct = UTradingManager::GetProductDataById(UIModel->GetClickedProductId());
	FItemInfoData ItemInfoData = UItemManager::GetItemInfoById(UIModel->GetClickedProductId());

	UTexture2D* LoadedImage = ItemInfoData.GetThumbnail().LoadSynchronous();
	ItemIcon->SetBrushFromTexture(LoadedImage);

	const AMainPlayerState* PS = GetController()->GetPlayerController()->GetPlayerState<AMainPlayerState>();

	switch (UIModel->GetTradingState())
	{
		case ETradingState::SELL:
		{
			Title->SetText(FText::FromString(FString::Printf(TEXT("판매하기: %s"), *ItemInfoData.GetDisplayName())));
	
			uint32 Price = ProductStruct.GetProductPrice() * ProductStruct.GetSellPriceRatio();
			Button->GetText()->SetText(FText::FromString(FString::FromInt(Price)));
			
			if (PS)
			{
				uint32 MaxCount = PS->GetInventoryComponent()->GetCurrentCount(UIModel->GetClickedProductId());
				CountSlider->SetMaxValue(MaxCount);
			}
			break;
		}
		default:
		{
			Title->SetText(FText::FromString(FString::Printf(TEXT("구매하기: %s"), *ItemInfoData.GetDisplayName())));
			
			uint32 Price = ProductStruct.GetProductPrice() * ProductStruct.GetBuyPriceRatio();
			Button->GetText()->SetText(FText::FromString(FString::FromInt(Price)));
			
			if (PS)
			{
				uint32 MaxCount = FMath::FloorToInt(PS->GetGold() / static_cast<float>(Price));
				CountSlider->SetMaxValue(MaxCount);
			}
			break;
		}
	}

	CountSlider->SetValue(1);
}

void UUIV_ProductBuyNotification::Increase()
{
	int Count = FMath::Clamp(CountSlider->GetValue() + 1, CountSlider->GetMinValue(), CountSlider->GetMaxValue());
	CountSlider->SetValue(Count);
}

void UUIV_ProductBuyNotification::Decrease()
{
	int Count = FMath::Clamp(CountSlider->GetValue() - 1, CountSlider->GetMinValue(), CountSlider->GetMaxValue());
	CountSlider->SetValue(Count);
}

void UUIV_ProductBuyNotification::OnCountValueChanged(float Value)
{
	const uint32 Count = FMath::FloorToInt(Value);
	UUIC_ProductBuyNotification* UIController = Cast<UUIC_ProductBuyNotification>(GetController());
	UIController->UpdateCount(Count);

	CountText->SetText(FText::FromString(FString::Printf(TEXT("x%d"), Count)));

	UUIM_ProductBuyNotification* UIModel = Cast<UUIM_ProductBuyNotification>(GetModel());
	FProductStruct ProductStruct = UTradingManager::GetProductDataById(UIModel->GetClickedProductId());

	AMainPlayerState* PS = GetController()->GetPlayerController()->GetPlayerState<AMainPlayerState>();

	switch (UIModel->GetTradingState())
	{
		case ETradingState::SELL:
		{
			FItemMetaInfo MetaInfo = PS->GetInventoryComponent()->GetFirstMetaInfo
			(UIModel->GetClickedProductId());
		
			float PriceRatio = UItemManager::GetPriceRatio(MetaInfo);
		
			uint32 Price = ProductStruct.GetProductPrice() * 
			ProductStruct.GetSellPriceRatio() * PriceRatio * Count;
		
			Button->GetText()->SetText(FText::FromString(FString::FromInt(Price)));
		}
			break;
		default:
		{
			uint32 Price = ProductStruct.GetProductPrice() * ProductStruct.GetBuyPriceRatio() * Count;
			Button->GetText()->SetText(FText::FromString(FString::FromInt(Price)));
		}
		break;
	}
}
