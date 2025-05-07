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

	void OnBite(TSoftObjectPtr<USkeletalMesh> Fish);

	void RemoveFish();

protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* LineAttachPoint;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* FishMesh;

	UPROPERTY(EditAnywhere)
	float ImpulseStrength = 20000.f;
};
