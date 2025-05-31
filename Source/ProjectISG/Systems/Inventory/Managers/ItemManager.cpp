#include "ItemManager.h"

#include "ProjectISG/Systems/Inventory/ItemData.h"
#include "ProjectISG/Utils/EnumUtil.h"

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

FItemInfoData& UItemManager::GetItemInfoById(const uint16 Id)
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

FString UItemManager::GetItemCategoryTextById(const uint16 Id)
{
	switch (GetItemInfoById(Id).GetItemType())
	{
	case EItemType::Undefined:
		{
			return TEXT("없음");
		}
	case EItemType::Build:
		{
			return TEXT("건축");
		}
	case EItemType::Consume:
		{
			return TEXT("소비");
		}
	case EItemType::Equipment:
		{
			return TEXT("장비");
		}
	case EItemType::Ingredient:
		{
			return TEXT("재료");
		}
	case EItemType::Interactive:
		{
			return TEXT("상호작용");
		}
	default:
		{
			return TEXT("없음");
		}
	}
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

bool UItemManager::IsInfiniteDurability(const uint16 Id)
{
	const FItemInfoData ItemInfoData = GetItemInfoById(Id);

	const FString* FindData = ItemInfoData.GetConstData().Find(
		EConstDataKey::MaxDurability);

	if (!FindData)
	{
		return false;
	}

	const FString FindDataRef = ItemInfoData.GetConstData().FindRef(
		EConstDataKey::MaxDurability);

	if (FindDataRef.IsEmpty())
	{
		return false;
	}

	if (!FindDataRef.IsNumeric())
	{
		return false;
	}

	const int32 Num = FCString::Atoi(*FindDataRef);

	return Num < 0;
}

uint16 UItemManager::GetGeneratedOtherItemIdById(const uint16 Id)
{
	const FItemInfoData ItemInfoData = GetItemInfoById(Id);

	const FString* FindData = ItemInfoData.GetConstData().Find(
		EConstDataKey::GeneratedItemId);

	if (!FindData)
	{
		return 0;
	}

	const FString FindDataRef = ItemInfoData.GetConstData().FindRef(
		EConstDataKey::GeneratedItemId);

	if (FindDataRef.IsEmpty())
	{
		return 0;
	}

	if (!FindDataRef.IsNumeric())
	{
		return 0;
	}

	return FCString::Atoi(*FindDataRef);
}

uint16 UItemManager::GetChanceBasedSpawnItemIdById(const uint16 Id)
{
	const FItemInfoData ItemInfoData = GetItemInfoById(Id);

	const FString* FindData = ItemInfoData.GetConstData().Find
		(EConstDataKey::ChanceBasedSpawnItemId);

	if (!FindData)
	{
		return 0;
	}

	const FString FindDataRef = ItemInfoData.GetConstData().FindRef
		(EConstDataKey::ChanceBasedSpawnItemId);

	if (FindDataRef.IsEmpty())
	{
		return 0;
	}

	if (!FindDataRef.IsNumeric())
	{
		return 0;
	}

	return FCString::Atoi(*FindDataRef);
}

FName UItemManager::GetSocketNameById(const uint16 Id)
{
	const FItemInfoData ItemInfoData = GetItemInfoById(Id);

	const FString* FindData = ItemInfoData.GetConstData().Find(
		EConstDataKey::SocketName);

	if (!FindData)
	{
		return NAME_None;
	}

	const FString FindDataRef = ItemInfoData.GetConstData().FindRef(
		EConstDataKey::SocketName);

	if (FindDataRef.IsEmpty())
	{
		return NAME_None;
	}

	return *FindDataRef;
}

EItemGrade UItemManager::GetItemGrade(const FItemMetaInfo& Info)
{
	if (!Info.GetMetaData().Contains(EMetaDataKey::ItemGrade))
	{
		return EItemGrade::None;
	}

	if (Info.GetMetaData()[EMetaDataKey::ItemGrade] == TEXT("Common"))
	{
		return EItemGrade::Common;
	}

	if (Info.GetMetaData()[EMetaDataKey::ItemGrade] == TEXT("Uncommon"))
	{
		return EItemGrade::Uncommon;
	}

	if (Info.GetMetaData()[EMetaDataKey::ItemGrade] == TEXT("Rare"))
	{
		return EItemGrade::Rare;
	}

	if (Info.GetMetaData()[EMetaDataKey::ItemGrade] == TEXT("Unique"))
	{
		return EItemGrade::Unique;
	}


	return EItemGrade::None;
}

FString UItemManager::GetItemGradeText(const FItemMetaInfo& Info)
{
	switch (GetItemGrade(Info))
	{
	case EItemGrade::Common:
		{
			return TEXT("커먼");
		}
	case EItemGrade::Uncommon:
		{
			return TEXT("언커먼");
		}
	case EItemGrade::Rare:
		{
			return TEXT("레어");
		}
	case EItemGrade::Unique:
		{
			return TEXT("유니크");
		}
	default:
		{
			return TEXT("없음");
		}
	}
}

void UItemManager::SetItemGrade(FItemMetaInfo& Info, const EItemGrade ItemGrade)
{
	Info.GetMetaData().Add(EMetaDataKey::ItemGrade,
	                       FEnumUtil::GetClassEnumKeyAsString(ItemGrade));
}

float UItemManager::GetPriceRatio(const FItemMetaInfo& Info)
{
	switch (GetItemGrade(Info))
	{
	case EItemGrade::Uncommon:
		return 1.2f;
	case EItemGrade::Rare:
		return 1.5f;
	case EItemGrade::Unique:
		return 2.0f;
	default:
		return 1.f;
	}
}
