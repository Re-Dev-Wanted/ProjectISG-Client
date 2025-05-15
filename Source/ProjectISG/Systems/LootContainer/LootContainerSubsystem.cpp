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

void ULootContainerSubsystem::CreateLootContainer(FGuid NewGuid, int32 Capacity)
{
	Data.AddContainer(NewGuid, Capacity);
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
	if (GetOwner()->HasAuthority())
	{
		Data.UpdateContainer(Guid, ItemInfo, Index);
	}
	else
	{
		Server_ChangeItem(Guid, FItemMetaInfo_Net(ItemInfo), Index);
	}

	return true;
}

void ULootContainerSubsystem::SwapItem(FGuid Guid, const uint16 Prev, const uint16 Next)
{
	if (GetOwner()->HasAuthority())
	{
		Data.SwapItemInContainer(Guid, Prev, Next);
	}
	else
	{
		Server_SwapItem(Guid, Prev, Next);
	}
}

void ULootContainerSubsystem::Server_CreateLootContainer_Implementation(
	FGuid NewGuid, uint16 Capacity)
{
	CreateLootContainer(NewGuid, Capacity);
}

void ULootContainerSubsystem::Server_SwapItem_Implementation(FGuid Guid,
	const uint16 Prev, const uint16 Next)
{
	Data.SwapItemInContainer(Guid, Prev, Next);
}

void ULootContainerSubsystem::Server_ChangeItem_Implementation(FGuid Guid,
	const FItemMetaInfo_Net& ItemInfo, const uint16 Index)
{
	FItemMetaInfo MetaInfo;
	ItemInfo.To(MetaInfo);
	
	Data.UpdateContainer(Guid, MetaInfo, Index);
}

void ULootContainerSubsystem::OnRep_UpdateData()
{
	Data.MarkArrayDirty();
}
