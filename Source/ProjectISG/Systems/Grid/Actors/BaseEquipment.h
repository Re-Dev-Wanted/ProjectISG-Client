// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/GAS/Common/Object/BaseActor.h"
#include "ProjectISG/Systems/Input/Interface/InteractionInterface.h"
#include "BaseEquipment.generated.h"

UCLASS()
class PROJECTISG_API ABaseEquipment : public ABaseActor, public IInteractionInterface
{
	GENERATED_BODY()

public:
	ABaseEquipment();

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* AnchorComp;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* MeshComp;

	virtual void OnInteractive(AActor* Causer) override {}

	virtual void OnInteractAction(AActor* Causer) override {}
	
};
