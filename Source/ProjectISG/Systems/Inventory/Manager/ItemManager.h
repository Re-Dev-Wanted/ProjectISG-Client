#pragma once

#include "ProjectISG/Systems/Inventory/ItemData.h"

class FItemManager
{
public:
	static FItemInfoData GetItemInfoById(const UWorld* World, const uint16 Id);
	static FItemMetaInfo GetInitialItemMetaDataById(
		const UWorld* World, const uint16 Id);
};
