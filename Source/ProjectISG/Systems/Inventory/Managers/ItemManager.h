#pragma once

#include "ProjectISG/Systems/Inventory/ItemData.h"
#include "ItemManager.generated.h"

UCLASS()
class UItemManager : public UObject
{
	GENERATED_BODY()

public:
	static void Initialize();
	static FItemInfoData GetItemInfoById(const uint16 Id);
	static FItemMetaInfo GetInitialItemMetaDataById(const uint16 Id);
	static bool IsItemCanHousing(const uint16 Id);
	static bool IsItemCanInteraction(const uint16 Id);

private:
	static TArray<FItemInfoData> ItemInfoList;
	static bool IsInitialize;
};
