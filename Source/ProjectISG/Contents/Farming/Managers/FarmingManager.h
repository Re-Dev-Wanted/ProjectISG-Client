// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Contents/Farming/FarmingStruct.h"
#include "UObject/Object.h"
#include "FarmingManager.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTISG_API UFarmingManager : public UObject
{
	GENERATED_BODY()

public:
	static void Initialize();

	static TArray<FFarmingStruct> GetFarmingObjectData()
	{
		return FarmingObjectData;
	}

	static FFarmingStruct GetDataByCropId(uint16 CropId);
	static FFarmingStruct GetDataByItemId(uint16 ItemId);

private:
	static TArray<FFarmingStruct> FarmingObjectData;
	static TMap<uint16, FFarmingStruct> FarmingObjectDataMap;
	static bool IsInitialized;
};
