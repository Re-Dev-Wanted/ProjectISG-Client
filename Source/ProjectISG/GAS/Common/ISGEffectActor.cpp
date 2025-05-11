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
                                          TSubclassOf<UGameplayEffect>
                                          GameplayEffectClass)
{
	UAbilitySystemComponent* TargetASC =
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	if (TargetASC)
	{
		check(GameplayEffectClass);

		FGameplayEffectContextHandle EffectContextHandle = TargetASC->
			MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
		const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->
			MakeOutgoingSpec(GameplayEffectClass, ActorLevel,
			                 EffectContextHandle);
		const FActiveGameplayEffectHandle ActiveGameplayEffectHandle = TargetASC
			->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

		const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->
		                                          DurationPolicy ==
			EGameplayEffectDurationType::Infinite;
		if (bIsInfinite && InfiniteEffectRemovePolicy ==
			EEffectRemovePolicy::RemoveOnEndOverlap)
		{
			// 효과 삭제전에 저장하기 위해 map에 저장함
			ActiveEffectHandles.Add(ActiveGameplayEffectHandle, TargetASC);
		}
	}
}

void AISGEffectActor::OnOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy ==
		EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}

	if (DurationEffectApplicationPolicy ==
		EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}

	if (InfiniteEffectApplicationPolicy ==
		EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
}

void AISGEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy ==
		EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}

	if (DurationEffectApplicationPolicy ==
		EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}

	if (InfiniteEffectApplicationPolicy ==
		EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}

	if (InfiniteEffectRemovePolicy == EEffectRemovePolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* TargetASC =
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(
				TargetActor);

		if (!IsValid(TargetASC))
		{
			return;
		}

		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		for (auto HandlePair : ActiveEffectHandles)
		{
			if (TargetASC == HandlePair.Value)
			{
				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
				HandlesToRemove.Add(HandlePair.Key);
			}
		}

		for (auto& Handle : HandlesToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}
	}
}
