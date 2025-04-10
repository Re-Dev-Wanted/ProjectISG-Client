#include "ItemManager.h"

#include "ProjectISG/Core/ISGGameInstance.h"
#include "ProjectISG/Systems/Inventory/ItemData.h"

FItemInfoData FItemManager::GetItemInfoById(const UWorld* World,
                                            const uint16 Id)
{
	const UISGGameInstance* GI = World->GetGameInstance<UISGGameInstance>();
	return GI->GetItemInfoList()[Id];
}

FItemMetaInfo FItemManager::GetInitialItemMetaDataById
(const UWorld* World, const uint16 Id)
{
	const FItemInfoData InitialData = GetItemInfoById(World, Id);
	// TODO: 만약 OptionData 기반으로 세팅이 필요하다면 InitialData 기반으로
	// 여기서 OptionData와 MetaData를 맞춰줄 것

	FItemMetaInfo NewMetaInfo;
	NewMetaInfo.SetId(Id);
	NewMetaInfo.SetCurrentCount(1);
	NewMetaInfo.SetMetaData(InitialData.GetMetaData());

	return NewMetaInfo;
}