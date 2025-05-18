// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/Interfaces/UIHandler.h"
#include "ProjectISG/Systems/Grid/Actors/Placement.h"
#include "SeatingFurniture.generated.h"

UCLASS()
class PROJECTISG_API ASeatingFurniture : public APlacement, public IUIHandler
{
	GENERATED_BODY()

public:
	virtual void OnInteractive(AActor* Causer) override;

	virtual bool GetCanInteractive() const override;

	virtual void OnInteractiveResponse(AActor* Causer) override;

	virtual FString GetInteractiveDisplayText() const override;

	virtual void OnClosed() override;

protected:
	bool bInteractive = true;
};
