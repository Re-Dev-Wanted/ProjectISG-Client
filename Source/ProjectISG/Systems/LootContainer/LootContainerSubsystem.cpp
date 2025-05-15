#include "LootContainerSubsystem.h"

#include "Net/UnrealNetwork.h"

ULootContainerSubsystem::ULootContainerSubsystem()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}

void ULootContainerSubsystem::InitializeComponent()
{
	Super::InitializeComponent();
	
	SetIsReplicated(true);
}

void ULootContainerSubsystem::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ULootContainerSubsystem, Data);
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

FItemMetaInfo ULootContainerSubsystem::GetItemMetaInfo(FGuid Guid, const uint16 Index)
{
	TArray<FItemMetaInfo> Items = GetContainerItems(Guid);

	return Items[Index];
}

bool ULootContainerSubsystem::ChangeItem(FGuid Guid, const FItemMetaInfo& ItemInfo, const uint16 Index)
{
	Data.UpdateContainer(Guid, ItemInfo, Index);
	return true;
}

void ULootContainerSubsystem::SwapItem(FGuid Guid, const uint16 Prev, const uint16 Next)
{
	Data.SwapItemInContainer(Guid, Prev, Next);
}
