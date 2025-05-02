#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Contents/Farming/BaseCrop.h"
#include "ProjectISG/Systems/Grid/Actors/Placement.h"
#include "HoedField.generated.h"

struct FItemInfoData;
class ABaseCrop;

USTRUCT()
struct FPlantedCrop
{
	GENERATED_BODY()

	UPROPERTY()
	TWeakObjectPtr<ABaseCrop> Crop = nullptr;

	UPROPERTY()
	uint16 CropId = 0;

	void Clear(bool Destroy)
	{
		if (Crop.IsValid() && Destroy)
		{
			ABaseCrop* RawActor = Crop.Get();
			RawActor->Destroy();
		}
		
		Crop = nullptr;
		CropId = 0;
	}

	bool IsValid() const
	{
		return Crop.IsValid() && CropId > 0;
	}
};

UCLASS()
class PROJECTISG_API AHoedField : public APlacement
{
	GENERATED_BODY()

public:
	virtual bool GetCanTouch() const override;

	virtual bool GetCanInteractive() const override;
	
	virtual void OnTouch(AActor* Causer) override;

	virtual void OnInteractive(AActor* Causer) override;

	virtual void OnInteractiveResponse() override;

	virtual FString GetDisplayText() const override;

	bool PlantCrop(FItemInfoData CropData, uint16 CropId);
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	FPlantedCrop PlantedCrop = FPlantedCrop();
};
