#include "AnimMontageManager.h"

bool UAnimMontageManager::IsInitialized;
TArray<FAnimMontageData> UAnimMontageManager::Datas;
TMap<EAnimMontageKey, FAnimMontageData> UAnimMontageManager::DataMap;

void UAnimMontageManager::Initialize()
{
	if (IsInitialized)
	{
		return;
	}

	IsInitialized = true;

	const static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("/Game/Systems/Animation/DT_AnimMontageData.DT_AnimMontageData"));

	if (DataTable.Succeeded())
	{
		TArray<FAnimMontageData*> TempItemInfoList;
		DataTable.Object->GetAllRows<FAnimMontageData>(TEXT(""), TempItemInfoList);

		for (const FAnimMontageData* InfoItem : TempItemInfoList)
		{
			Datas.Add(*InfoItem);
			DataMap.Add(InfoItem->GetKey(), *InfoItem);
		}
	}
}

FAnimMontageData UAnimMontageManager::GetDataByKey(EAnimMontageKey Key)
{
	if (DataMap.Contains(Key))
	{
		return DataMap[Key];
	}

	return FAnimMontageData();
}

TObjectPtr<UAnimMontage> UAnimMontageManager::GetMontage(EAnimMontageKey Key,
	ECharacterName Character)
{
	if (DataMap.Contains(Key))
	{
		if (DataMap[Key].GetMontageData().Contains(Character))
		{
			return DataMap[Key].GetMontageData()[Character];
		}
	}

	return nullptr;
}
