#include "ItemManager.h"

#include "ProjectISG/Systems/Inventory/ItemData.h"

void FItemManager::Initialize()
{
	const static ConstructorHelpers::FObjectFinder<UDataTable>
		ItemInfoDataTable(TEXT(
			"/Script/Engine.DataTable'/Game/Systems/Inventory/DT_ItemList.DT_ItemList'"));

	if (ItemInfoDataTable.Succeeded())
	{
		TArray<FItemInfoData*> TempItemInfoList;
		ItemInfoDataTable.Object->GetAllRows<FItemInfoData>(
			TEXT(""), TempItemInfoList);

		for (const FItemInfoData* InfoItem : TempItemInfoList)
		{
			ItemInfoList.Add(*InfoItem);
		}
	}
}

FItemInfoData FItemManager::GetItemInfoById(const uint16 Id)
{
	return ItemInfoList[Id];
}

FItemMetaInfo FItemManager::GetInitialItemMetaDataById(const uint16 Id)
{
	const FItemInfoData InitialData = GetItemInfoById(Id);

	FItemMetaInfo NewMetaInfo;
	NewMetaInfo.SetId(Id);
	NewMetaInfo.SetCurrentCount(1);
	NewMetaInfo.SetMetaData(InitialData.GetMetaData());

	return NewMetaInfo;
}
