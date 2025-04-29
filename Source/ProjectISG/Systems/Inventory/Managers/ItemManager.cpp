#include "ItemManager.h"

#include "ProjectISG/Systems/Inventory/ItemData.h"

bool UItemManager::IsInitialize;
TArray<FItemInfoData> UItemManager::ItemInfoList;

void UItemManager::Initialize()
{
	if (IsInitialize)
	{
		return;
	}

	IsInitialize = true;

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

FString UItemManager::GetItemUsingType(const uint16 Id)
{
	const FItemInfoData ItemInfoData = GetItemInfoById(Id);

	const FString* FindData = ItemInfoData.GetConstData().Find(
		EConstDataKey::ItemUseType);

	if (!FindData)
	{
		return FString();
	}

	return ItemInfoData.GetConstData().FindRef(EConstDataKey::ItemUseType);
}

bool UItemManager::IsItemCanHousing(const uint16 Id)
{
	const FItemInfoData ItemInfoData = GetItemInfoById(Id);

	const FString* FindData = ItemInfoData.GetConstData().Find(
		EConstDataKey::ItemUseType);
	if (!FindData)
	{
		return false;
	}

	const FString FindDataRef = ItemInfoData.GetConstData().FindRef(
		EConstDataKey::ItemUseType);

	if (!FindDataRef.Equals(TEXT("Housing")))
	{
		return false;
	}

	return true;
}

bool UItemManager::IsItemCanInteraction(const uint16 Id)
{
	const FItemInfoData ItemInfoData = GetItemInfoById(Id);

	const EItemType itemType = ItemInfoData.GetItemType();

	if (itemType != EItemType::Interactive)
	{
		return false;
	}

	return true;
}

FName UItemManager::GetSocketNameById(const uint16 Id)
{
	const FItemInfoData ItemInfoData = GetItemInfoById(Id);

	const FString* FindData = ItemInfoData.GetConstData().Find(EConstDataKey::SocketName);

	if (!FindData)
	{
		return NAME_None;
	}

	const FString FindDataRef = ItemInfoData.GetConstData().FindRef(EConstDataKey::SocketName);

	if (FindDataRef.IsEmpty())
	{
		return NAME_None;
	}

	return *FindDataRef;
}
