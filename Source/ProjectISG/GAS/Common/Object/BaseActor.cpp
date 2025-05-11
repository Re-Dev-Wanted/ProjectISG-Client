#include "BaseActor.h"

#include "ProjectISG/GAS/Common/ISGAbilitySystemComponent.h"

ABaseActor::ABaseActor()
{
	AbilitySystemComponent = CreateDefaultSubobject<UISGAbilitySystemComponent>(
		"Ability System Component");
}

void ABaseActor::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	AbilitySystemComponent->Initialize(InitialData);
}

UAbilitySystemComponent* ABaseActor::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
