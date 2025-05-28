// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Systems/Water/Actors/FloatingActor.h"
#include "Bobber.generated.h"

UCLASS()
class PROJECTISG_API ABobber : public AFloatingActor
{
	GENERATED_BODY()

public:
	ABobber();

	void SuggestProjectileVelocity(const FVector& StartLocation, const FVector& EndLocation);

	void OnBite(UStaticMesh* Fish);

	void RemoveFish();

protected:
	virtual float GetBuoyancyScale() const override;
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* LineAttachPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* FishMeshComp;

	UPROPERTY(EditAnywhere)
	float ImpulseStrength = 20000.f;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> SFX_Bite;
};
