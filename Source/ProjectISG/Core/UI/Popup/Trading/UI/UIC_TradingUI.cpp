// Fill out your copyright notice in the Description page of Project Settings.


#include "UIC_TradingUI.h"

#include "EnhancedInputComponent.h"
#include "UIV_TradingUI.h"
#include "Components/TextBlock.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"


class AMainPlayerState;
class AMainPlayerCharacter;

void UUIC_TradingUI::BindInputAction(UEnhancedInputComponent* InputComponent)
{
	Super::BindInputAction(InputComponent);

	InputComponent->BindAction(CloseTradingUI, ETriggerEvent::Triggered, this,
	                           &ThisClass::OnCloseTradingUI);
}

void UUIC_TradingUI::OnCloseTradingUI()
{
	AMainPlayerController* PC = Cast<AMainPlayerController>(GetPlayerController());
	PC->PopUI();
	PC->PushUI(EUIName::Gameplay_MainHUD);
}

void UUIC_TradingUI::UpdateGoldText()
{
	AMainPlayerController* PC = Cast<AMainPlayerController>(GetPlayerController());
	AMainPlayerState* PS = PC->GetPlayerState<AMainPlayerState>();
	UUIV_TradingUI* TradingUIView = Cast<UUIV_TradingUI>(GetView());

	if (PC && PS && TradingUIView)
	{
		FString Str = FString::Printf(TEXT("Gold : %d"), PS->GetGold());
		TradingUIView->GetGoldText()->SetText(FText::FromString(Str)); 
	}
}
