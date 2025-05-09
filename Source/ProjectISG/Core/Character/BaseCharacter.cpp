#include "BaseCharacter.h"

#include "LevelSequencePlayer.h"
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

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ABaseCharacter::PlayCinematic(ULevelSequencePlayer* Cinematic)
{
	UE_LOG(LogTemp, Display, TEXT("클라, 서버 여부 %d"), HasAuthority());
	Cinematic->Play();
}

void ABaseCharacter::Client_PlayCinematic_Implementation(
	ULevelSequencePlayer* Cinematic)
{
	UE_LOG(LogTemp, Display, TEXT("너 호출 안되는거 같아... %p"), Cinematic);
	PlayCinematic(Cinematic);
}
