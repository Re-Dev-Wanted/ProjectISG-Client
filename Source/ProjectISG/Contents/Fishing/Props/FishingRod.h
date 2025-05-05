// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Systems/Water/Actors/DynamicEquipment.h"
#include "FishingRod.generated.h"

class ABobber;
class UCableComponent;

UCLASS()
class PROJECTISG_API AFishingRod : public ADynamicEquipment
{
	GENERATED_BODY()

public:
	AFishingRod();

protected:
	virtual void BeginPlay() override;

	virtual void Destroyed() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABobber> BobberFactory;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* SocketComp;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* PocketSocketComp;
	
	UPROPERTY(VisibleAnywhere)
	UCableComponent* Thread;
	
	UPROPERTY()
	TObjectPtr<ABobber> Bobber;

public:
	virtual void Tick(float DeltaTime) override;
};
