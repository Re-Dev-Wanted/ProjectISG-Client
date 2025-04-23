#include "BaseUIController.h"

#include "BaseUIView.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"

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
		BindInputAction(EnhancedInputComponent);
	}
}

void UBaseUIController::BindInputAction(UEnhancedInputComponent* InputComponent)
{
}

void UBaseUIController::AppearUI()
{
	View->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UBaseUIController::DisappearUI()
{
	View->SetVisibility(ESlateVisibility::Hidden);
	ClearInputMappingContext();
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

		UEnhancedInputComponent* EnhancedInputComponent = Cast<
			UEnhancedInputComponent>(PlayerController->InputComponent);

		if (!EnhancedInputComponent)
		{
			return;
		}
		BindInputAction(EnhancedInputComponent);
	}
}
