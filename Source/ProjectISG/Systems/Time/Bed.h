// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "Bed.generated.h"

UCLASS()
class PROJECTISG_API ABed : public AActor
{
	GENERATED_BODY()

public:
	ABed();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnBedBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	                       AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                       int32 OtherBodyIndex, bool bFromSweep,
	                       const FHitResult& SweepResult);

	UFUNCTION()
	void OnBedEndOverlap(UPrimitiveComponent* OverlappedComponent,
	                     AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                     int32 OtherBodyIndex);

private:
	UFUNCTION()
	void MovePlayerToBed();

public:
	SETTER(class AMainPlayerCharacter*, MainPlayer);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting,
		meta = (AllowPrivateAccess = true))
	class UBoxComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting,
		meta = (AllowPrivateAccess = true))
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting,
		meta = (AllowPrivateAccess = true))
	class ATimeManager* TimeManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting,
		meta = (AllowPrivateAccess = true))
	class AMainPlayerCharacter* MainPlayer;
};
