// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/GAS/Common/Object/BaseActor.h"
#include "ProjectISG/Systems/Water/Interfaces/BuoyantInterface.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "FloatingActor.generated.h"

class UBoxComponent;
class UBuoyancyComponent;

DECLARE_DYNAMIC_DELEGATE(FOnEnterWater);

UCLASS()
class PROJECTISG_API AFloatingActor : public ABaseActor, public IBuoyantInterface
{
	GENERATED_BODY()

public:
	GETTER(TObjectPtr<UBoxComponent>, Root)
	GETTER(TObjectPtr<UBuoyancyComponent>, BuoyancyComponent)

	AFloatingActor();

	FOnEnterWater OnEnterWater;

	virtual bool CanApplyBuoyancy() const override;
	virtual float GetBuoyancyScale() const override;
	virtual float GetActorBottomZ() const override;

	virtual void SetCollisionAndPhysicsEnabled(bool bIsEnabled);

	virtual void EnterWater();

	void SetDamping(float Linear, float Angular);

protected:
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Root;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;
	
	UPROPERTY(VisibleAnywhere)
	UBuoyancyComponent* BuoyancyComponent;
	
};
