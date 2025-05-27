// Fill out your copyright notice in the Description page of Project Settings.


#include "UIV_TradingUI.h"

#include "UIC_TradingUI.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/UI/HUD/Inventory/InventoryList.h"
#include "ProjectISG/Core/UI/HUD/Inventory/Module/InventorySlotDragDropOperation.h"

bool UUIV_TradingUI::NativeOnDrop(const FGeometry& InGeometry,
                                  const FDragDropEvent& InDragDropEvent,
                                  UDragDropOperation* InOperation)
{
	UInventorySlotDragDropOperation* ItemDragDropOperation = Cast<
		UInventorySlotDragDropOperation>(InOperation);

	if (ItemDragDropOperation)
	{
		if (ItemListView)
		{
			UUIC_TradingUI* TradingUIController = Cast<UUIC_TradingUI>(
				GetController());
			if (TradingUIController)
			{
				AMainPlayerController* PC = Cast<AMainPlayerController>(
					TradingUIController->GetPlayerController());
				if (PC)
				{
					PC->PushUI(EUIName::Modal_SellNotification);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("플레이어 컨트롤러 캐스팅 실패"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("거래 UI 컨트롤러 캐스팅 실패"));
			}
		}
	}
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

void UUIV_TradingUI::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void UUIV_TradingUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (OpenFlag == false)
	{
		UUIC_TradingUI* TradingUIController = Cast<UUIC_TradingUI>(
					GetController());
		TradingUIController->UpdateInventory();
		OpenFlag = true;
	}
}
