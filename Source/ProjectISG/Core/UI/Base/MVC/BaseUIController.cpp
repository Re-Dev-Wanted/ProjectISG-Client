#include "BaseUIController.h"

#include "BaseUIModel.h"
#include "BaseUIView.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/UI/UIEnum.h"

void UBaseUIController::InitializeController(UBaseUIView* NewView,
                                             UBaseUIModel* NewModel)
{
	View = NewView;
	Model = NewModel;
}

void UBaseUIController::InitializeSettingToPlayerController(
	APlayerController* PC)
{
	PlayerController = PC;
	ChangeInputActionToUI(true);
}

void UBaseUIController::ChangeInputActionToUI(const bool IsBindAction)
{
	if (!IsInputAccess)
	{
		return;
	}

	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		PlayerController->GetPawn());
	if (!Player)
	{
		return;
	}

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
			PlayerController->GetLocalPlayer()))
	{
		Subsystem->RemoveMappingContext(Player->GetDefaultMappingContext());
		Subsystem->AddMappingContext(UIMappingContext, 0);

		UEnhancedInputComponent* EnhancedInputComponent = Cast<
			UEnhancedInputComponent>(PlayerController->InputComponent);

		if (!EnhancedInputComponent)
		{
			return;
		}

		if (IsBindAction)
		{
			BindInputAction(EnhancedInputComponent);
		}
	}
}

void UBaseUIController::BindInputAction(UEnhancedInputComponent* InputComponent)
{
}

void UBaseUIController::AppearUI(const EUILayer Layer)
{
	View->SetVisibility(ESlateVisibility::Visible);

	if (Layer != EUILayer::Gameplay)
	{
		ChangeInputActionToUI(false);

		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(GetView()->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

		PlayerController->SetInputMode(InputMode);
		PlayerController->SetShowMouseCursor(true);
	}
}

void UBaseUIController::DisappearUI()
{
	View->SetVisibility(ESlateVisibility::Hidden);

	ClearInputMappingContext();

	const FInputModeGameOnly InputMode;
	PlayerController->SetInputMode(InputMode);
	PlayerController->SetShowMouseCursor(false);
}

void UBaseUIController::PopUIFromPlayerController()
{
	AMainPlayerController* PC = Cast<AMainPlayerController>(PlayerController);
	PC->PopUI();
}

void UBaseUIController::ClearInputMappingContext()
{
	if (!IsInputAccess)
	{
		return;
	}
	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		PlayerController->GetPawn());
	if (!Player)
	{
		return;
	}

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
			PlayerController->GetLocalPlayer()))
	{
		Subsystem->RemoveMappingContext(UIMappingContext);
		Subsystem->AddMappingContext(Player->GetDefaultMappingContext(), 0);
	}
}
