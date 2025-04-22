#include "BaseCharacter.h"
#include "ProjectISG/GAS/Common/ISGAbilitySystemComponent.h"

ABaseCharacter::ABaseCharacter()
{
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseCharacter::InitializePrimaryAttributes()
{
	// ApplyGameplayEffectSpecToTarget 사용법
	check(IsValid(GetAbilitySystemComponent()));
	check(DefaultPrimaryAttributes);
	FGameplayEffectContextHandle ContextHandel = GetAbilitySystemComponent()->
		MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->
		MakeOutgoingSpec(DefaultPrimaryAttributes, 1.f, ContextHandel);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(
		*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

// void ABaseCharacter::AddCharacterAbilities()
// {
// 	UISGAbilitySystemComponent* ISGASC = CastChecked<UISGAbilitySystemComponent>(AbilitySystemComponent);
// 	if (HasAuthority() == false) return;
//
// 	ISGASC->AddCharacterAbilities(StartupAbilities);
// }

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
