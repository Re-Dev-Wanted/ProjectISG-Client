#include "ISGGameInstance.h"

#include "ProjectISG/Contents/Cooking/Managers/CookingManager.h"
#include "ProjectISG/Contents/Fishing/Managers/FishingManager.h"
#include "ProjectISG/Contents/Trading/TradingManager.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"

UISGGameInstance::UISGGameInstance()
{
	// TODO: 생성자에서 런타임 Init() 으로 옮겨도 무방
	// 단 내부 로직의 일부 변경이 필요할 수 있음
	UItemManager::Initialize();
	UTradingManager::Initialize();
	UFishingManager::Initialize();
}

void UISGGameInstance::Init()
{
	Super::Init();

	UCookingManager::Initialize();
}
