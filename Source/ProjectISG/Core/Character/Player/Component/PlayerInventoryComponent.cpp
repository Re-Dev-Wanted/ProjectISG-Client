#include "PlayerInventoryComponent.h"

#include "EnhancedInputComponent.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"


UPlayerInventoryComponent::UPlayerInventoryComponent()
{
	bWantsInitializeComponent = true;
}

void UPlayerInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPlayerInventoryComponent::InitializeComponent()
{
	Super::InitializeComponent();

	AMainPlayerCharacter* OwnerPlayer = Cast<AMainPlayerCharacter>(GetOwner());
	OwnerPlayer->OnInputBindingNotified.AddDynamic(
		this, &ThisClass::BindingInputActions);
}

void UPlayerInventoryComponent::BindingInputActions(
	UEnhancedInputComponent* EnhancedInputComponent)
{
	EnhancedInputComponent->BindAction(ToggleInventoryInputAction,
	                                   ETriggerEvent::Triggered, this,
	                                   &ThisClass::ToggleInventory);
}

void UPlayerInventoryComponent::ToggleInventory()
{
	AMainPlayerController* PC = Cast<AMainPlayerController>(
		GetOwner()->GetInstigatorController());

	IsOpenedInventory = !IsOpenedInventory;
	PC->ToggleInventoryUI(IsOpenedInventory);
}
