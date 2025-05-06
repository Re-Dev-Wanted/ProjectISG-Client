// Fill out your copyright notice in the Description page of Project Settings.

#include "FishingManager.h"

#include "ProjectISG/Utils/ArrayUtil.h"

bool UFishingManager::IsInitialized;
TArray<FFishData> UFishingManager::Datas;
TMap<uint16, FFishData> UFishingManager::DataMap;

void UFishingManager::Initialize()
{
	if (IsInitialized)
	{
		return;
	}

	IsInitialized = true;

	const static ConstructorHelpers::FObjectFinder<UDataTable>
		ItemInfoDataTable(TEXT("/Game/Contents/Fishing/Data/DT_FishData.DT_FishData"));

	if (ItemInfoDataTable.Succeeded())
	{
		TArray<FFishData*> TempItemInfoList;
		ItemInfoDataTable.Object->GetAllRows<FFishData>(TEXT(""), TempItemInfoList);

		for (const FFishData* InfoItem : TempItemInfoList)
		{
			Datas.Add(*InfoItem);
			DataMap.Add(InfoItem->GetId(), *InfoItem);
		}
	}
}

FFishData UFishingManager::GetRandomData()
{
	if (!IsInitialized)
	{
		Initialize();
	}

	TArray Arr(Datas);
	ArrayUtil::KnuthShuffle(Arr);

	return Arr[0];
}

FFishData UFishingManager::GetDataById(uint16 Id)
{
	if (DataMap.Contains(Id))
	{
		return DataMap[Id];
	}

	return FFishData();
}

FFishData UFishingManager::GetDataByItemId(uint16 ItemId)
{
	for (FFishData Data : Datas)
	{
		if (Data.GetItemId() == ItemId)
		{
			return Data;
		}
	}

	return FFishData();
}


