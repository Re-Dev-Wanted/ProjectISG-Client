// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Actor.h"
#include "GC_FarmPlow.generated.h"

UCLASS()
class PROJECTISG_API AGC_FarmPlow : public AGameplayCueNotify_Actor
{
	GENERATED_BODY()

public:
	AGC_FarmPlow();

protected:
	virtual bool OnExecute_Implementation(AActor* MyTarget,
	                                      const FGameplayCueParameters&
	                                      Parameters) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Options",
		meta = (AllowPrivateAccess = true))
	TObjectPtr<USoundBase> FarmingSound;

	UPROPERTY()
	TObjectPtr<UAudioComponent> AudioComponent;

	void SpawnSound();
};
