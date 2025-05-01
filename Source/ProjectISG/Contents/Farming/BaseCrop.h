// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/GAS/Common/Object/BaseActor.h"
#include "ProjectISG/Systems/Input/Interface/InteractionInterface.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "BaseCrop.generated.h"

UCLASS()
class PROJECTISG_API ABaseCrop : public ABaseActor, public IInteractionInterface

{
	GENERATED_BODY()

public:
	ABaseCrop();

	virtual void OnInteractive(AActor* Causer) override;

	virtual void OnInteractAction(AActor* Causer) override;

protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(
		TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void Tick(float DeltaTime) override;

	virtual bool GetCanInteractive() const override;

	virtual FString GetDisplayText() const override;

	virtual void OnInteractiveResponse() override;

	UFUNCTION()
	void UpdateGrowTimeBySleep();

public:
	GETTER_SETTER(int32, CropTotalGrowDay);
	GETTER_SETTER(int32, WaterDuration);
	GETTER(uint16, CropId);
	
	UPROPERTY(Replicated)
	bool CanInteractive;
private:
	void CheckGrowTime();

	void CheckWaterDurationTime();

	UFUNCTION(Reliable, Server)
	void Server_CropIsMature();

	UFUNCTION(Reliable, NetMulticast)
	void NetMulticast_ChangeCropMeshToMature();

	void CropIsGetWater();

#pragma region Settings
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings,
		meta = (AllowPrivateAccess = true))
	class UBoxComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings,
		meta = (AllowPrivateAccess = true))
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings,
		meta = (AllowPrivateAccess = true))
	class USceneComponent* InteractionPos;

	UPROPERTY(EditAnywhere, Category = Settings,
		meta = (AllowPrivateAccess = true))
	uint16 CropId;

	UPROPERTY()
	class ATimeManager* TimeManager = nullptr;
	
	UPROPERTY(Replicated)
	FString DisplayText;
#pragma endregion

#pragma region Grow
	UPROPERTY(EditAnywhere, Category = Grow)
	float CropStartGrowTime;

	UPROPERTY(EditAnywhere, Category = Grow)
	int32 CropStartGrowDay;

	UPROPERTY(EditAnywhere, Category = Grow)
	int32 CropGrowTime = 0;

	UPROPERTY(EditAnywhere, Category = Grow)
	int32 CropRemainGrowTime = 0;

	UPROPERTY(EditAnywhere, Category = Grow)
	int32 CropTotalGrowDay = 0;

	UPROPERTY(EditAnywhere, Category = Grow)
	int32 WaterDuration = 0;

	UPROPERTY(Replicated, EditAnywhere, Category = Grow)
	bool bIsMature = false;

	UPROPERTY(Replicated, EditAnywhere, Category = Grow)
	bool bIsGetWater = false;
#pragma endregion
};
