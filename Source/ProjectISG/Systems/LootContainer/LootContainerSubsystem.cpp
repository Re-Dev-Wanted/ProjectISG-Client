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

FGuid ULootContainerSubsystem::CreateLootContainer(int32 Capacity)
{
	return Data.AddContainer(Capacity);
}

TArray<FItemMetaInfo> ULootContainerSubsystem::GetContainerItems(FGuid Guid)
{
	return Data.GetItems(Guid);
}
