// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ISGEffectActor.generated.h"


class UGameplayEffect;

UCLASS()
class PROJECTISG_API AISGEffectActor : public AActor
{
	GENERATED_BODY()

public:
	AISGEffectActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects", meta = (AllowPrivateAccess = true)) 
	TSubclassOf<UGameplayEffect> InstanceGameplayEffectClass;


};
