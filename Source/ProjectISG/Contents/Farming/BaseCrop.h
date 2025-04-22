// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "BaseCrop.generated.h"

UCLASS()
class PROJECTISG_API ABaseCrop : public AActor
{
	GENERATED_BODY()

public:
	ABaseCrop();

protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void Tick(float DeltaTime) override;

	virtual void CheckGrowTime();
	
	UFUNCTION()
	void CropBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                      int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	GETTER_SETTER(int32, CropTotalGrowDay);
	GETTER_SETTER(bool, bIsGetWater);
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings,
		meta = (AllowPrivateAccess = true))
	class UBoxComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings,
		meta = (AllowPrivateAccess = true))
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = Settings,
		meta = (AllowPrivateAccess = true))
	uint16 CropId;
	
	UPROPERTY(EditAnywhere, Category = Grow,
		meta = (AllowPrivateAccess = true))
	FTimerHandle GrowTimerHandle;

	UPROPERTY()
	class ATimeManager* TimeManager = nullptr;

	UPROPERTY(EditAnywhere, Category = Grow)
	float CropStartGrowTime;

	UPROPERTY(EditAnywhere, Category = Grow)
	int32 CropStartGrowDay;	
	
	UPROPERTY(EditAnywhere, Category = Grow)
	int32 CropTotalGrowDay = 0;

	UPROPERTY(Replicated, EditAnywhere, Category = Grow)
	bool bIsMature = false;

	UPROPERTY(Replicated, EditAnywhere, Category = Grow)
	bool bIsGetWater = false;	
};
