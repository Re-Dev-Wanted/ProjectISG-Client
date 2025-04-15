#include "ISGGameInstance.h"

#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"
#include "ProjectISG/Systems/Logging/Managers/LoggingManager.h"

UISGGameInstance::UISGGameInstance()
{
	UItemManager::Initialize();
	ULoggingManager::Get()->Init(GetWorld());
}
