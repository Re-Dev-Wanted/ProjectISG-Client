#include "ItemManager.h"

#include "ProjectISG/Systems/Inventory/ItemData.h"

TArray<FItemInfoData> UItemManager::ItemInfoList;

void UItemManager::Initialize()
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

FItemInfoData UItemManager::GetItemInfoById(const uint16 Id)
{
	return ItemInfoList[Id];
}

FItemMetaInfo UItemManager::GetInitialItemMetaDataById(const uint16 Id)
{
	const FItemInfoData InitialData = GetItemInfoById(Id);

	FItemMetaInfo NewMetaInfo;
	NewMetaInfo.SetId(Id);
	NewMetaInfo.SetCurrentCount(1);
	NewMetaInfo.SetMetaData(InitialData.GetMetaData());

	return NewMetaInfo;
}

bool UItemManager::IsItemCanHousing(const uint16 Id)
{
	const FItemInfoData ItemInfoData = GetItemInfoById(Id);

	const FString* FindData = ItemInfoData.GetConstData().
	                                       Find(
		                                       EConstDataKey::ItemUseType);
	if (!FindData)
	{
		return false;
	}

	const FString FindDataRef = ItemInfoData.GetConstData().
	                                         FindRef(
		                                         EConstDataKey::ItemUseType);

	if (!FindDataRef.Equals(TEXT("Housing")))
	{
		return false;
	}

	return true;
}
