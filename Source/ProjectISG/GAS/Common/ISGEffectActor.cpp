// Fill out your copyright notice in the Description page of Project Settings.


#include "ISGEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ISGAbilitySystemComponent.h"


AISGEffectActor::AISGEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root")));
}

void AISGEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AISGEffectActor::ApplyEffectToTarget(AActor* TargetActor,
	TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	if (TargetASC)
	{
		check(GameplayEffectClass);
		
		FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
		FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1.f, EffectContextHandle);
		TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	}
}



