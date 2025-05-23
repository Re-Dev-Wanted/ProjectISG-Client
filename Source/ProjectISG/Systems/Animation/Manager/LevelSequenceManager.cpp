#include "LevelSequenceManager.h"

#include "ProjectISG/Core/PlayerState/MainPlayerState.h"

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

	const UDataTable* DataTable = LoadObject<UDataTable>(nullptr,
		TEXT(
			"/Game/Systems/Animation/DT_LevelSequenceData.DT_LevelSequenceData"));

	if (!DataTable)
	{
		return;
	}

	TArray<FLevelSequenceData*> TempItemInfoList;
	DataTable->GetAllRows<FLevelSequenceData>(
		TEXT(""), TempItemInfoList);

	for (const FLevelSequenceData* InfoItem : TempItemInfoList)
	{
		Datas.Add(*InfoItem);
		DataMap.Add(InfoItem->GetKey(), *InfoItem);
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
	const AMainPlayerState* PS, const ELevelSequenceKey Key)
{
	Initialize();

	if (DataMap.Contains(Key))
	{
		if (DataMap[Key].GetSequenceData().Contains(PS->GetCharacterName()))
		{
			return DataMap[Key].GetSequenceData()[PS->GetCharacterName()];
		}
	}

	return nullptr;
}
