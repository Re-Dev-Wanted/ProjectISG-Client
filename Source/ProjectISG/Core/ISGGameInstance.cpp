#include "ISGGameInstance.h"

#include "ProjectISG/Systems/Inventory/Manager/ItemManager.h"

UISGGameInstance::UISGGameInstance()
{
	UItemManager::Initialize();
}
