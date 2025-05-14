#include "ISGGameInstance.h"

#include "ProjectISG/Contents/Cooking/Managers/CookingManager.h"
#include "ProjectISG/Contents/Fishing/Managers/FishingManager.h"
#include "ProjectISG/Contents/Trading/TradingManager.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"
#include "ProjectISG/Systems/QuestStory/Manager/QuestStoryManager.h"

UISGGameInstance::UISGGameInstance()
{
	// TODO: 런타임에 할 수 있게끔 처리할 필요 있음
	// 사유: 게임을 껏다 켜야만 데이터가 리로드 된다는 문제점.
	UItemManager::Initialize();
	UCookingManager::Initialize();
	UTradingManager::Initialize();
	UFishingManager::Initialize();
}

void UISGGameInstance::Init()
{
	Super::Init();

	UQuestStoryManager::Initialize();
}
