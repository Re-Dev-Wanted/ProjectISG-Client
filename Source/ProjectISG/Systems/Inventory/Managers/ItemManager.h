#pragma once

#include "ProjectISG/Systems/Inventory/ItemData.h"
#include "ItemManager.generated.h"

UCLASS()
class UItemManager : public UObject
{
	GENERATED_BODY()

public:
	static void Initialize();
	static FItemInfoData& GetItemInfoById(const uint16 Id);
	static FItemMetaInfo GetInitialItemMetaDataById(const uint16 Id);
	static FString GetItemUsingType(const uint16 Id);
	static bool IsItemCanHousing(const uint16 Id);
	static bool IsItemCanInteraction(const uint16 Id);
	static bool IsInfiniteDurability(const uint16 Id);
	static uint16 GetGeneratedOtherItemIdById(const uint16 Id);
	static uint16 GetChanceBasedSpawnItemIdById(const uint16 Id);
	static FName GetSocketNameById(const uint16 Id);
	static EItemGrade GetItemGrade(const FItemMetaInfo& Info);
	static FString GetItemGradeText(const FItemMetaInfo& Info);
	static void SetItemGrade(FItemMetaInfo& Info, const EItemGrade ItemGrade);
	static float GetPriceRatio(const FItemMetaInfo& Info);

private:
	static TArray<FItemInfoData> ItemInfoList;
	static bool IsInitialize;
};
