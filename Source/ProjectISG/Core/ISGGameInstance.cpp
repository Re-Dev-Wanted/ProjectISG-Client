#include "ISGGameInstance.h"

UISGGameInstance::UISGGameInstance()
{
	LoadItemData();
}

void UISGGameInstance::LoadItemData()
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
