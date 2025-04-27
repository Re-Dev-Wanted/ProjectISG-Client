#include "ISGGameInstance.h"

#include "ProjectISG/Contents/Cooking/Managers/CookingManager.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"

UISGGameInstance::UISGGameInstance()
{
	UItemManager::Initialize();
	UCookingManager::Initialize();
}
