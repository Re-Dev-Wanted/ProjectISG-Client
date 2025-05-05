// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Systems/Grid/Actors/BaseEquipment.h"
#include "FishingRod.generated.h"

UCLASS()
class PROJECTISG_API AFishingRod : public ABaseEquipment
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFishingRod();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
