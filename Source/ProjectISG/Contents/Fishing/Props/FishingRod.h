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

	UFUNCTION()
	void OnStartFishing();

	UFUNCTION()
	void OnEventBite();

	FTimerHandle FishingTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABobber> BobberFactory;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* SocketComp;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* PocketSocketComp;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* CastingStartPoint;
	
	UPROPERTY(VisibleAnywhere)
	UCableComponent* Thread;
	
	UPROPERTY()
	TObjectPtr<ABobber> Bobber;

public:
	virtual void Tick(float DeltaTime) override;

	virtual bool GetCanTouch() const override;
	
	virtual FString GetDisplayText() const override;
	
	virtual void OnTouch(AActor* Causer) override;
	
	virtual void OnTouchResponse() override;
	
	void StartCasting(FVector Destination);
};
