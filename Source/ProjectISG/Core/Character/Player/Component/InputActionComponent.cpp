#include "InputActionComponent.h"
#include "EnhancedInputComponent.h"

#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"

UInputActionComponent::UInputActionComponent()
{
	bWantsInitializeComponent = true;
}


void UInputActionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInputActionComponent::InitializeComponent()
{
	Super::InitializeComponent();

	AMainPlayerCharacter* OwnerPlayer = Cast<AMainPlayerCharacter>(GetOwner());

	OwnerPlayer->OnInputBindingNotified.AddDynamic(
		this, &ThisClass::BindingInputActions);
}

void UInputActionComponent::BindingInputActions(
	UEnhancedInputComponent* EnhancedInputComponent)
{
	EnhancedInputComponent->BindAction(ToggleQuestInputAction,
	                                   ETriggerEvent::Triggered, this,
	                                   &ThisClass::OnToggleQuestUI);
}

void UInputActionComponent::OnToggleQuestUI()
{
	const AMainPlayerCharacter* OwnerPlayer = Cast<AMainPlayerCharacter>(
		GetOwner());

	Cast<AMainPlayerController>(OwnerPlayer->GetController())->PushUI(
		EUIName::Popup_QuestListUI);
}
