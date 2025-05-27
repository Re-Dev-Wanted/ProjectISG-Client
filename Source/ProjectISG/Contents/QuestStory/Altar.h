// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/GAS/Common/Object/BaseInteractiveActor.h"
#include "ProjectISG/Systems/Grid/Actors/Placement.h"
#include "Altar.generated.h"

UCLASS()
class PROJECTISG_API AAltar : public APlacement
{
	GENERATED_BODY()

public:
	AAltar();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void OnInteractive(AActor* Causer) override;

	virtual FString GetInteractiveDisplayText() const override;

	virtual bool GetCanInteractive() const override;
	
private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true),
		Category = Setting)
	uint32 OfferingFoodId;

};