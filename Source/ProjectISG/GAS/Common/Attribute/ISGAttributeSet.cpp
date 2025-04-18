// Fill out your copyright notice in the Description page of Project Settings.


#include "ISGAttributeSet.h"

#include "Net/UnrealNetwork.h"

UISGAttributeSet::UISGAttributeSet()
{
	InitHealth(100.f);
	InitMaxHealth(100.f);
}

void UISGAttributeSet::GetLifetimeReplicatedProps(
	TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UISGAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UISGAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
}

void UISGAttributeSet::OnRep_Health(const FGameplayAttributeData OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UISGAttributeSet, Health, OldHealth);
}

void UISGAttributeSet::OnRep_MaxHealth(
	const FGameplayAttributeData OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UISGAttributeSet, MaxHealth, OldMaxHealth);
}
