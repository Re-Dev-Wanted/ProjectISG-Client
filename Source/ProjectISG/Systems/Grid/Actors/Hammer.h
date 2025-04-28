// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEquipment.h"
#include "ProjectISG/GAS/Common/Object/BaseActor.h"
#include "ProjectISG/Systems/Input/Interface/InteractionInterface.h"
#include "Hammer.generated.h"

UCLASS()
class PROJECTISG_API AHammer : public ABaseEquipment
{
	GENERATED_BODY()

public:
	AHammer();

	virtual void OnInteractive(AActor* Causer) override;

	virtual void OnInteractAction(AActor* Causer) override;

	virtual void OnTouchAction(AActor* Causer) override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
