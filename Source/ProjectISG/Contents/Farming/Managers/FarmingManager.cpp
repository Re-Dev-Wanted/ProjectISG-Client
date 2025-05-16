// Fill out your copyright notice in the Description page of Project Settings.


#include "FarmingManager.h"

bool UFarmingManager::IsInitialized;
TArray<FFarmingStruct> UFarmingManager::FarmingObjectData;
TMap<uint16, FFarmingStruct> UFarmingManager::FarmingObjectDataMap;

void UFarmingManager::Initialize()
{
	if (IsInitialized)
	{
		return;
	}

	IsInitialized = true;

	const static ConstructorHelpers::FObjectFinder<UDataTable>
		FarmingObjectDataTable(
			TEXT(
				"'/Game/Contents/Farming/Data/DT_FarmingData.DT_FarmingData'"));

	if (FarmingObjectDataTable.Succeeded())
	{
		TArray<FFarmingStruct*> TempFarmingObjectDataList;
		FarmingObjectDataTable.Object->GetAllRows<FFarmingStruct>(
			TEXT(""), TempFarmingObjectDataList);

		for (const FFarmingStruct* InfoFarmingObject :
		     TempFarmingObjectDataList)
		{
			FarmingObjectData.Add(*InfoFarmingObject);
			FarmingObjectDataMap.Add(InfoFarmingObject->GetCropId(),
			                         *InfoFarmingObject);
		}
	}
}

FFarmingStruct UFarmingManager::GetDataByCropId(uint16 CropId)
{
	if (FarmingObjectDataMap.Contains(CropId))
	{
		return FarmingObjectDataMap[CropId];
	}

	return FFarmingStruct();
}

FFarmingStruct UFarmingManager::GetDataByItemId(uint16 ItemId)
{
	for (FFarmingStruct Data : FarmingObjectData)
	{
		if (Data.GetItemId() == ItemId)
		{
			return Data;
		}
	}

	return FFarmingStruct();
}
