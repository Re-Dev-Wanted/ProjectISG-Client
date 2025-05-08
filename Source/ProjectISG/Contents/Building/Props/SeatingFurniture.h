// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Systems/Grid/Actors/Placement.h"
#include "SeatingFurniture.generated.h"

UCLASS()
class PROJECTISG_API ASeatingFurniture : public APlacement
{
	GENERATED_BODY()

public:
	virtual void OnInteractive(AActor* Causer) override;

	virtual bool GetCanInteractive() const override;

	virtual FString GetDisplayText() const override;

protected:
	bool bInteractive = true;

	virtual FVector GetStartInteractPoint() const override;
};
