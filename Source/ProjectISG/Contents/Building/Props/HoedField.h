#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Contents/Farming/BaseCrop.h"
#include "ProjectISG/Systems/Grid/Actors/Placement.h"
#include "HoedField.generated.h"

class UNiagaraSystem;
struct FItemMetaInfo;
struct FItemInfoData;
class ABaseCrop;
class UMaterialInstanceDynamic;

USTRUCT(BlueprintType)
struct FPlantedCrop
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	ABaseCrop* Crop = nullptr;

	UPROPERTY()
	uint16 CropId = 0;

	void Clear(bool Destroy)
	{
		if (Crop && Destroy)
		{
			Crop->Destroy();
		}
		
		Crop = nullptr;
		CropId = 0;
	}

	bool IsValid() const
	{
		return Crop != nullptr && CropId > 0;
	}
	
};

UCLASS()
class PROJECTISG_API AHoedField : public APlacement
{
	GENERATED_BODY()

public:
	AHoedField();

	virtual void OnSpawned() override;
	
	virtual bool GetCanTouch() const override;

	virtual bool GetCanInteractive() const override;
	
	virtual void OnTouch(AActor* Causer) override;

	virtual FString GetTouchDisplayText(AActor* Causer) const override;
	
	virtual void OnTouchResponse(AActor* Causer) override;

	bool PlantCrop(FItemInfoData CropData, uint16 CropId);

	UFUNCTION(Server, Reliable)
	void SetWet(bool Watering);

	UFUNCTION(Server, Reliable)
	void DestroyCropAndClearData();
	
protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_UpdateState();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings,
	meta = (AllowPrivateAccess = true))
	class USceneComponent* InteractionPos;

	UPROPERTY(EditAnywhere)
	class UMaterialInstance* FieldMaterial;

	UPROPERTY(Replicated, EditAnywhere)
	FPlantedCrop PlantedCrop = FPlantedCrop();

	UPROPERTY(ReplicatedUsing = OnRep_UpdateState)
	bool IsWet = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings,
	meta = (AllowPrivateAccess = true, ClampMin = 0, ClampMax = 100))
	float ChanceBasedPercent = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings)
	TObjectPtr<UNiagaraSystem> Effect;
	
	void UpdateState();

	UFUNCTION(Server, Reliable)
	void Server_SetMaterialInstanceParam();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetMaterialInstanceParam(float Multiply, float Hue, float 
	Metal);
};
