// Fill out your copyright notice in the Description page of Project Settings.


#include "UIC_TradingUI.h"

#include "EnhancedInputComponent.h"
#include "UIM_TradingUI.h"
#include "UIV_TradingUI.h"
#include "Components/TextBlock.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/PlayerInventoryComponent.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Core/UI/HUD/Inventory/InventoryList.h"

class AMainPlayerState;
class AMainPlayerCharacter;

void UUIC_TradingUI::BindInputAction(UEnhancedInputComponent* InputComponent)
{
	Super::BindInputAction(InputComponent);

	InputComponent->BindAction(CloseTradingUI, ETriggerEvent::Triggered, this,
	                           &ThisClass::OnCloseTradingUI);

	const UUIV_TradingUI* TradingView = Cast<UUIV_TradingUI>(GetView());

	TradingView->GetInventoryList()->OnDragDetectedNotified.AddDynamic(
		this, &UUIC_TradingUI::DetectDragItem);
}

void UUIC_TradingUI::OnCloseTradingUI()
{
	AMainPlayerController* PC = Cast<AMainPlayerController>(
		GetPlayerController());

	UUIV_TradingUI* TradingUIView = Cast<UUIV_TradingUI>(GetView());
	TradingUIView->SetOpenFlag(false);
	PC->PopUI();
}

void UUIC_TradingUI::UpdateGoldText()
{
	AMainPlayerController* PC = Cast<AMainPlayerController>(
		GetPlayerController());
	AMainPlayerState* PS = PC->GetPlayerState<AMainPlayerState>();
	UUIV_TradingUI* TradingUIView = Cast<UUIV_TradingUI>(GetView());

	if (PC && PS && TradingUIView)
	{
		FString Str = FString::Printf(TEXT("Gold : %dG"), PS->GetGold());
		TradingUIView->GetGoldText()->SetText(FText::FromString(Str));
	}
}

void UUIC_TradingUI::UpdateInventory()
{
	AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(GetPlayerController()->GetPawn());
	if (Player && Player->GetPlayerInventoryComponent())
	{
		Player->GetPlayerInventoryComponent()->UpdateInventorySlotItemData();
	}
}

void UUIC_TradingUI::DetectDragItem(uint16 ItemId)
{
	UUIM_TradingUI* TradingModel = Cast<UUIM_TradingUI>(GetModel());
	TradingModel->SetClickedInventoryItem(ItemId);
}
