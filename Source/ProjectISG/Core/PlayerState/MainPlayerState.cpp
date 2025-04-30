#include "MainPlayerState.h"

#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/GAS/Common/ISGAbilitySystemComponent.h"
#include "ProjectISG/GAS/Common/Attribute/ISGAttributeSet.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"

AMainPlayerState::AMainPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UISGAbilitySystemComponent>(
		"Ability System Component");

	AttributeSet = CreateDefaultSubobject<UISGAttributeSet>(
		TEXT("Attribute Set"));


	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(
		"Inventory Component");
}

UAbilitySystemComponent* AMainPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

bool AMainPlayerState::CanBuyProduct(int32 ProductPrice)
{
	if (Gold >= ProductPrice)
	{
		return true;
	}
	return false;
}

void AMainPlayerState::BeginPlay()
{
	Super::BeginPlay();
}

void AMainPlayerState::InitializeData()
{
	GetInventoryComponent()->InitializeItemData();
}
