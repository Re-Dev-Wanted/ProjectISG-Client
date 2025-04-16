#include "MainPlayerState.h"
#include "ProjectISG/GAS/Common/ISGAbilitySystemComponent.h"

AMainPlayerState::AMainPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UISGAbilitySystemComponent>(
		"Ability System Component");
}

UAbilitySystemComponent* AMainPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AMainPlayerState::BeginPlay()
{
	Super::BeginPlay();
}
