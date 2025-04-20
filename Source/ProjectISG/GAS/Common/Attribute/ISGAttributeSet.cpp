// Fill out your copyright notice in the Description page of Project Settings.


#include "ISGAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

UISGAttributeSet::UISGAttributeSet()
{
}

void UISGAttributeSet::GetLifetimeReplicatedProps(
	TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UISGAttributeSet, Health, COND_None,
	                               REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UISGAttributeSet, MaxHealth, COND_None,
	                               REPNOTIFY_Always);
}

void UISGAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute,
                                          float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// attribute 값을 변경하기 직전에 실행되는 함수
	if (Attribute == GetHealthAttribute())
	{
		// 값을 바꾸기 전에 clamp로 최대와 최소를 넘지 않도록 설정해준다.
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
}

void UISGAttributeSet::PostGameplayEffectExecute(
	const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
}

void UISGAttributeSet::SetEffectProperties(
	const struct FGameplayEffectModCallbackData& Data,
	FEffectProperties& Props) const
{
	Props.EffectContextHandle = Data.EffectSpec.
	                                 GetContext();
	Props.SourceASC = Props.EffectContextHandle.
	                        GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid()
		&& Props.SourceASC
		        ->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.
		                          SourceASC->AbilityActorInfo->AvatarActor.
		                          Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->
		                               PlayerController.Get();

		if (Props.SourceController == nullptr && Props.SourceAvatarActor !=
			nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		if (Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(
				Props.SourceController->GetPawn());
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->
		AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.
		                               Get();
		Props.TargetController = Data.Target.AbilityActorInfo->
		                              PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC =
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(
				Props.TargetAvatarActor);
	}
}

void UISGAttributeSet::OnRep_Health(
	const FGameplayAttributeData OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UISGAttributeSet, Health, OldHealth);
}

void UISGAttributeSet::OnRep_MaxHealth(
	const FGameplayAttributeData OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UISGAttributeSet, MaxHealth, OldMaxHealth);
}
