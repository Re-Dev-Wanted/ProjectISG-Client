#include "BaseUIController.h"

#include "BaseUIView.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/UI/UIEnum.h"

void UBaseUIController::StartShowUI(const EUILayer Layer)
{
	CurrentLayer = Layer;

	const UWidgetAnimation* StartAnimation = GetView()->
		GetDefaultStartAnimation();
	GetView()->SetVisibility(ESlateVisibility::Visible);

	if (!StartAnimation)
	{
		AppearUI();
		return;
	}

	GetView()->PlayAnimation(GetView()->GetDefaultStartAnimation());
}

void UBaseUIController::EndShowUI(const bool IsWidget)
{
	const UWidgetAnimation* EndAnimation = GetView()->
		GetDefaultEndAnimation();
	if (!EndAnimation)
	{
		if (IsWidget)
		{
			View->SetVisibility(ESlateVisibility::Hidden);
			return;
		}

		DisappearUI();
		return;
	}
	GetView()->PlayAnimation(GetView()->GetDefaultEndAnimation());
}

void UBaseUIController::InitializeController(UBaseUIView* NewView,
                                             UBaseUIModel* NewModel)
{
	View = NewView;
	Model = NewModel;

	if (GetView()->GetDefaultTickAnimation())
	{
		GetView()->TickAnimationEndNotified.BindDynamic(
			this, &ThisClass::OnFinishDefaultTickAnimation);
		GetView()->BindToAnimationFinished(GetView()->GetDefaultTickAnimation(),
		                                   GetView()->TickAnimationEndNotified);

		GetView()->PlayAnimation(GetView()->GetDefaultTickAnimation());
	}

	if (GetView()->GetDefaultStartAnimation())
	{
		GetView()->StartAnimationFinishNotified.BindDynamic(
			this, &ThisClass::AppearUI);
		GetView()->BindToAnimationFinished(
			GetView()->GetDefaultStartAnimation(),
			GetView()->StartAnimationFinishNotified);
	}

	if (GetView()->GetDefaultEndAnimation())
	{
		GetView()->EndAnimationFinishNotified.BindDynamic(
			this, &ThisClass::DisappearUI);
		GetView()->BindToAnimationFinished(
			GetView()->GetDefaultEndAnimation(),
			GetView()->EndAnimationFinishNotified);
	}
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

	if (!UIMappingContext)
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

void UBaseUIController::AppearUI()
{
	AppearUI(GetCurrentLayer());
}

void UBaseUIController::AppearUI(const EUILayer Layer)
{
	if (Layer == EUILayer::Gameplay)
	{
		const FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
		PlayerController->SetShowMouseCursor(false);

		return;
	}

	ChangeInputActionToUI(false);

	FInputModeGameAndUI InputMode;
	InputMode.SetWidgetToFocus(GetView()->TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputMode);
	PlayerController->SetShowMouseCursor(true);
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

void UBaseUIController::OnFinishDefaultTickAnimation()
{
	// 애니메이션이 갑자기 사라질 수 있지만, 실제 비즈니스 로직에 문제가 되어서는 안된다.
	if (!GetView()->GetDefaultTickAnimation())
	{
		return;
	}

	GetView()->PlayAnimation(GetView()->GetDefaultTickAnimation());
}
