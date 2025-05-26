// Fill out your copyright notice in the Description page of Project Settings.


#include "UIC_ExitInteractUI.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "UIM_ExitInteractUI.h"
#include "UIV_ExitInteractUI.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/UI/HUD/Interactive/InteractiveItemUI.h"
#include "ProjectISG/GAS/Common/Tag/ISGGameplayTag.h"

void UUIC_ExitInteractUI::AppearUI()
{
	Super::AppearUI();

	const FInputModeGameOnly InputMode;
	GetPlayerController()->SetInputMode(InputMode);
	GetPlayerController()->SetShowMouseCursor(false);
}

void UUIC_ExitInteractUI::SetUI(const FString& Key, const FString& Text)
{
	const UUIV_ExitInteractUI* InteractView = Cast<UUIV_ExitInteractUI>(
		GetView());
	InteractView->GetInteractiveUI()->SetInteractive(Key, Text);
}

void UUIC_ExitInteractUI::BindInputAction(
	UEnhancedInputComponent* InputComponent)
{
	Super::BindInputAction(InputComponent);

	InputComponent->BindAction(ExitInteractAction, ETriggerEvent::Started, this
								, &ThisClass::ExitInteract);

	InputComponent->BindAction(RotateAction, ETriggerEvent::Triggered, this
								, &ThisClass::Look);
}

void UUIC_ExitInteractUI::ExitInteract()
{
	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		GetPlayerController()->GetPawn());

	if (Player)
	{
		FGameplayEventData EventData;
		EventData.EventTag = ISGGameplayTags::Interactive_Active_Exit;
		EventData.Instigator = Player;

		Player->GetAbilitySystemComponent()->HandleGameplayEvent(
			ISGGameplayTags::Interactive_Active_Exit, &EventData);
	}

	PopUIFromPlayerController();
}

void UUIC_ExitInteractUI::Look(const FInputActionValue& Value)
{
	const FVector2d LookToValue = Value.Get<FVector2d>();

	AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		GetPlayerController()->GetPawn());

	if (!Player)
	{
		return;
	}

	Player->AddControllerYawInput(LookToValue.X);
	Player->AddControllerPitchInput(LookToValue.Y);
}
