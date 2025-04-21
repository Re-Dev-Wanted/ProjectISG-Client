#include "MainPlayerComponent.h"

#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"

UMainPlayerComponent::UMainPlayerComponent()
{
}

void UMainPlayerComponent::InitializeComponent()
{
	Super::InitializeComponent();

	AMainPlayerCharacter* OwnerPlayer = Cast<AMainPlayerCharacter>(GetOwner());

	OwnerPlayer->OnInputBindingNotified.AddDynamic(
		this, &ThisClass::BindingInputActions);
}

void UMainPlayerComponent::BindingInputActions(
	UEnhancedInputComponent* EnhancedInputComponent)
{
}
