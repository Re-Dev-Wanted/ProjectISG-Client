#include "LevelSequenceManager.h"

bool ULevelSequenceManager::IsInitialized;
TArray<FLevelSequenceData> ULevelSequenceManager::Datas;
TMap<ELevelSequenceKey, FLevelSequenceData> ULevelSequenceManager::DataMap;

void ULevelSequenceManager::Initialize()
{
	if (IsInitialized)
	{
		return;
	}

	IsInitialized = true;

	const static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("/Game/Systems/Animation/DT_LevelSequenceData.DT_LevelSequenceData"));

	if (DataTable.Succeeded())
	{
		TArray<FLevelSequenceData*> TempItemInfoList;
		DataTable.Object->GetAllRows<FLevelSequenceData>(TEXT(""), TempItemInfoList);

		for (const FLevelSequenceData* InfoItem : TempItemInfoList)
		{
			Datas.Add(*InfoItem);
			DataMap.Add(InfoItem->GetKey(), *InfoItem);
		}
	}
}

FLevelSequenceData ULevelSequenceManager::GetDataByKey(ELevelSequenceKey Key)
{
	if (DataMap.Contains(Key))
	{
		return DataMap[Key];
	}

	return FLevelSequenceData();
}

TObjectPtr<ULevelSequence> ULevelSequenceManager::GetLevelSequence(
	ELevelSequenceKey Key, ECharacterName Character)
{
	if (DataMap.Contains(Key))
	{
		if (DataMap[Key].GetSequenceData().Contains(Character))
		{
			return DataMap[Key].GetSequenceData()[Character];
		}
	}

	return nullptr;
}
