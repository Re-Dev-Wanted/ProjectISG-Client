// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaterVolume.generated.h"

class AFloatingActor;

UCLASS()
class PROJECTISG_API AWaterVolume : public AActor
{
	GENERATED_BODY()

public:
	AWaterVolume();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* VolumeCollision;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* WaterSurface;

	UPROPERTY()
	TSet<AFloatingActor*> FloatingActors;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> RipplesFactory;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
