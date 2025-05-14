#include "ISGGameInstance.h"

#include "ProjectISG/Contents/Cooking/Managers/CookingManager.h"
#include "ProjectISG/Contents/Fishing/Managers/FishingManager.h"
#include "ProjectISG/Contents/Trading/TradingManager.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"
#include "ProjectISG/Utils/SessionUtil.h"

UISGGameInstance::UISGGameInstance()
{
	UItemManager::Initialize();
	UCookingManager::Initialize();
	UTradingManager::Initialize();
	UFishingManager::Initialize();
}

void UISGGameInstance::Init()
{
	Super::Init();

	FSessionUtil::Init();
}
