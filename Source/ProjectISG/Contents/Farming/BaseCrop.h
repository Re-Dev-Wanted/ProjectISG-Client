// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/GAS/Common/Object/BaseInteractiveActor.h"
#include "ProjectISG/Systems/Inventory/ItemData.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "BaseCrop.generated.h"

UENUM(BlueprintType)
enum class ECropState :uint8
{
	None = 0,
	Seedling = 1,
	Sprout = 2,
	Stem = 3,
	Mature = 4
};

DECLARE_MULTICAST_DELEGATE(FOnDryField);
DECLARE_MULTICAST_DELEGATE(FHarvestCrop);

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

	virtual FString GetInteractiveDisplayText() const override;

	virtual void OnInteractiveResponse(AActor* Causer) override;

	UFUNCTION()
	void UpdateGrowTimeBySleep();

	UFUNCTION()
	void OnRep_UpdateState();

public:
	GETTER(ECropState, CurrentState)
	GETTER_SETTER(int32, CropTotalGrowDay);
	GETTER_SETTER(int32, WaterDuration);
	GETTER(uint16, ItemId);

	FOnDryField OnDryField;

	FHarvestCrop HarvestCrop;

private:
	void CheckGrowTime();

	void CheckStateByRemainGrowTime();

	void ChangeCurrentCropState(ECropState State);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_ChangeCurrentCropState(ECropState State);

	void CheckWaterDurationTime();

	UFUNCTION(Reliable, Server)
	void Server_CropIsMature();

	UFUNCTION(Reliable, NetMulticast)
	void NetMulticast_ChangeCropMeshToMature();

	UFUNCTION(Server, Reliable)
	void Server_FieldIsDried();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_FieldIsDried();

	EItemGrade SetItemGrade();

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
	uint16 ItemId;

	UPROPERTY(EditAnywhere, Category = Settings,
		meta = (AllowPrivateAccess = true))
	uint16 MatureFarmingObjectId;

	UPROPERTY()
	class ATimeManager* TimeManager = nullptr;
#pragma endregion

#pragma region Grow
	UPROPERTY(ReplicatedUsing = OnRep_UpdateState, EditAnywhere,
		Category = Grow)
	ECropState CurrentState = ECropState::None;

	UPROPERTY(EditAnywhere, Category = Grow)
	float CropStartGrowTime;

	UPROPERTY(EditAnywhere, Category = Grow)
	int32 CropStartGrowDay;

	UPROPERTY(EditAnywhere, Category = Grow)
	int32 CropGrowTime = 0;

	UPROPERTY(EditAnywhere, Category = Grow)
	int32 CropGrowTimeSum = 0;

	UPROPERTY(EditAnywhere, Category = Grow)
	int32 CropTotalGrowTime;

	UPROPERTY(EditAnywhere, Category = Grow)
	int32 CropTotalGrowDay = 1;

	UPROPERTY(EditAnywhere, Category = Grow)
	int32 WaterDuration = 12;

	UPROPERTY(Replicated, EditAnywhere, Category = Grow)
	bool bIsGetWater = false;

	UPROPERTY(EditAnywhere, Category = Grow)
	int32 CropBecomeSprout;

	UPROPERTY(EditAnywhere, Category = Grow)
	int32 CropBecomeStem;

#pragma endregion
};
