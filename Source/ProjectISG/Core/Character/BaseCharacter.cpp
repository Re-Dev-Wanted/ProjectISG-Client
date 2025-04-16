#include "BaseCharacter.h"
#include "ProjectISG/GAS/Common/ISGAbilitySystemComponent.h"

ABaseCharacter::ABaseCharacter()
{
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
