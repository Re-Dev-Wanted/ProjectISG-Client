// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AT_Watering.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFindSeedEndNotified);


UCLASS()
class PROJECTISG_API UAT_Watering: public UAbilityTask
{
	GENERATED_BODY()
	
protected:
	virtual void Activate() override;
	
	virtual void TickTask(float DeltaTime) override;

private:
	void FindSeed(FVector start, FVector end, FVector boxHalfSize);

public:
	static UAT_Watering* InitialEvent(UGameplayAbility* OwningAbility);
	
	FFindSeedEndNotified FindSeedDelegate;

private:
	UPROPERTY()
	class AMainPlayerCharacter* player = nullptr;

	UPROPERTY()
	class ABaseCrop* FoundSeed = nullptr;

	UPROPERTY()
	float WateringDistance = 100.f;
	

};
