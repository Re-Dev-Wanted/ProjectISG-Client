#include "BasePawn.h"


ABasePawn::ABasePawn()
{
}

void ABasePawn::BeginPlay()
{
	Super::BeginPlay();
}

UAbilitySystemComponent* ABasePawn::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

