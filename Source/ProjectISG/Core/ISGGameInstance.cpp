#include "ISGGameInstance.h"

#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"

UISGGameInstance::UISGGameInstance()
{
	UItemManager::Initialize();
}
