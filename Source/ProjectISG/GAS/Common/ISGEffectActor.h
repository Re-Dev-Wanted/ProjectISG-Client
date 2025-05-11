// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "ISGEffectActor.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;

UENUM(BlueprintType)
enum class EEffectApplicationPolicy : uint8
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply,
};

UENUM(BlueprintType)
enum class EEffectRemovePolicy : uint8
{
	RemoveOnEndOverlap,
	DoNotRemove,
};

UCLASS()
class PROJECTISG_API AISGEffectActor : public AActor
{
	GENERATED_BODY()

public:
	AISGEffectActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor,
	                         TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects",
		meta = (AllowPrivateAccess = true))
	bool bDestroyOnEffectRemoval = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects",
		meta = (AllowPrivateAccess = true))
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects",
		meta = (AllowPrivateAccess = true))
	EEffectApplicationPolicy InstantEffectApplicationPolicy =
		EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects",
		meta = (AllowPrivateAccess = true))
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects",
		meta = (AllowPrivateAccess = true))
	EEffectApplicationPolicy DurationEffectApplicationPolicy =
		EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects",
		meta = (AllowPrivateAccess = true))
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects",
		meta = (AllowPrivateAccess = true))
	EEffectApplicationPolicy InfiniteEffectApplicationPolicy =
		EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects",
		meta = (AllowPrivateAccess = true))
	EEffectRemovePolicy InfiniteEffectRemovePolicy =
		EEffectRemovePolicy::RemoveOnEndOverlap;

	UPROPERTY()
	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*>
	ActiveEffectHandles;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects",
		meta = (AllowPrivateAccess = true))
	float ActorLevel = 1.f;
};
