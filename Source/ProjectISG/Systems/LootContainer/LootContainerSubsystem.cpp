#include "LootContainerSubsystem.h"

void ULootContainerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void ULootContainerSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void ULootContainerSubsystem::LoadAllDataAsync()
{
}

FGuid ULootContainerSubsystem::CreateLootContainer()
{
	return Data.AddContainer();
}
