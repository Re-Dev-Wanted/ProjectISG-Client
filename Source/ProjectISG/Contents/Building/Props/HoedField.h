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

	void Clear(bool destory)
	{
		if (Crop.IsValid() && destory)
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
	virtual void OnTouch(AActor* Causer) override;

	bool PlantCrop(FItemInfoData CropData, uint16 CropId);
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	FPlantedCrop PlantedCrop = FPlantedCrop();
};
