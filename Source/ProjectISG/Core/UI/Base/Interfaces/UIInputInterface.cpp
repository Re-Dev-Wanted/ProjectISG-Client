#include "UIInputInterface.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "ProjectISG/Core/Controller/MainPlayerController.h"


void IUIInputInterface::InitializeUIInput(APlayerController* BasePC)
{
	PlayerController = BasePC;

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
			BasePC->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultUIMappingContext, 0);

		UEnhancedInputComponent* EnhancedInputComponent = Cast<
			UEnhancedInputComponent>(BasePC->InputComponent);

		InitializeInputData(Subsystem, EnhancedInputComponent);
	}
}

void IUIInputInterface::InitializeInputData(
	UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem,
	UEnhancedInputComponent* EnhancedInputComponent)
{
}

void IUIInputInterface::EscapeUI()
{
	if (AMainPlayerController* PC = Cast<AMainPlayerController>(
		PlayerController))
	{
		PC->PopUI();
	}
}
