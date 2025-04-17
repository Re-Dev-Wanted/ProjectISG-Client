#include "MainPlayerState.h"

#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/GAS/Common/ISGAbilitySystemComponent.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"

AMainPlayerState::AMainPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UISGAbilitySystemComponent>(
		"Ability System Component");

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(
		"Inventory Component");
}

UAbilitySystemComponent* AMainPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AMainPlayerState::BeginPlay()
{
	Super::BeginPlay();
}

void AMainPlayerState::InitializeData()
{
	GetInventoryComponent()->InitializeItemData();
	Cast<AMainPlayerController>(GetPlayerController())->InitializeHUD();
}
