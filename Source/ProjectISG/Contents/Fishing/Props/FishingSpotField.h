// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Systems/Input/Interface/InteractionInterface.h"
#include "FishingSpotField.generated.h"

class UBoxComponent;

UCLASS()
class PROJECTISG_API AFishingSpotField : public AActor, public IInteractionInterface
{
	GENERATED_BODY()

public:
	AFishingSpotField();

	virtual bool GetCanTouch() const override;

	virtual FString GetDisplayText() const override;
	
	virtual void OnTouch(AActor* Causer) override;

	virtual void OnTouchResponse() override;

protected:
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Root = nullptr;
	
};
