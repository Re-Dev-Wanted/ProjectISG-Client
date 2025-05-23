// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Systems/Input/Interface/InteractionInterface.h"
#include "FishingSpotField.generated.h"

class UCapsuleComponent;
class UBoxComponent;

UCLASS()
class PROJECTISG_API AFishingSpotField : public AActor, public IInteractionInterface
{
	GENERATED_BODY()

public:
	AFishingSpotField();

	virtual bool GetCanTouch() const override;

	virtual FString GetTouchDisplayText(AActor* Causer) const override;
	
	virtual void OnTouch(AActor* Causer) override;

	virtual void OnTouchResponse(AActor* Causer) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Root = nullptr;

	// UPROPERTY(VisibleAnywhere)
	// UCapsuleComponent* BlockCollision = nullptr;
	
};
