// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/GAS/Common/Object/BaseInteractiveActor.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "BaseCrop.generated.h"

UENUM(BlueprintType)
enum class ECropState :uint8
{
	Seedling = 0,
	Sprout = 1,
	Mature = 2
};

DECLARE_MULTICAST_DELEGATE(FOnDryField);

UCLASS()
class PROJECTISG_API ABaseCrop : public ABaseInteractiveActor

{
	GENERATED_BODY()

public:
	ABaseCrop();

	virtual void OnInteractive(AActor* Causer) override;

	void CropIsGetWater();

	void SetCurrentState(ECropState State);

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

	UFUNCTION()
	void OnRep_UpdateState();
	
public:
	GETTER(ECropState, CurrentState)
	GETTER_SETTER(int32, CropTotalGrowDay);
	GETTER_SETTER(int32, WaterDuration);
	GETTER(uint16, CropId);

	FOnDryField OnDryField;
	
private:
	void CheckGrowTime();

	void CheckWaterDurationTime();

	UFUNCTION(Reliable, Server)
	void Server_CropIsMature();

	UFUNCTION(Reliable, NetMulticast)
	void NetMulticast_ChangeCropMeshToMature();

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
#pragma endregion

#pragma region Grow
	UPROPERTY(ReplicatedUsing = OnRep_UpdateState)
	ECropState CurrentState = ECropState::Seedling;
	
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
	bool bIsGetWater = false;
#pragma endregion
};
