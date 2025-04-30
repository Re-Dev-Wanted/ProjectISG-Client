// Fill out your copyright notice in the Description page of Project Settings.


#include "UIC_TradingUI.h"

#include "EnhancedInputComponent.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"


class AMainPlayerCharacter;

void UUIC_TradingUI::BindInputAction(UEnhancedInputComponent* InputComponent)
{
	Super::BindInputAction(InputComponent);

	InputComponent->BindAction(CloseTradingUI, ETriggerEvent::Triggered, this,
	                           &ThisClass::OnCloseTradingUI);
}

void UUIC_TradingUI::OnCloseTradingUI()
{
	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		GetPlayerController()->GetPawn());
	if (Player)
	{
		AMainPlayerController* PC = Player->GetController<AMainPlayerController>(); 
		PC->PopUI();
		PC->PushUI(EUIName::Gameplay_MainHUD);
	}
}
