#pragma once

#include "ProjectISG/Systems/Inventory/ItemData.h"

class FItemManager
{
public:
	static void Initialize();
	static FItemInfoData GetItemInfoById(const uint16 Id);
	static FItemMetaInfo GetInitialItemMetaDataById(const uint16 Id);

private:
	static TArray<FItemInfoData> ItemInfoList;
};
