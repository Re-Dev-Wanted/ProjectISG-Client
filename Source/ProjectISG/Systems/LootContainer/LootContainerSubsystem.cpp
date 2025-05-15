#include "LootContainerSubsystem.h"

#include "Kismet/KismetSystemLibrary.h"
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

void ULootContainerSubsystem::CreateLootContainer(AActor* Causer, FGuid NewGuid, 
int32 Capacity)
{
	if (Causer->HasAuthority())
	{
		Data.AddContainer(NewGuid, Capacity);

		Multicast_CreateLootContainer(NewGuid, Capacity);
	}
	else
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("흠1"));
		Server_CreateLootContainer(NewGuid, Capacity);
	}
}

TArray<FItemMetaInfo> ULootContainerSubsystem::GetContainerItems(FGuid Guid)
{
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("%d"),
	 Data.GetItems(Guid).Num()));
	
	return Data.GetItems(Guid);
}

FItemMetaInfo ULootContainerSubsystem::GetItemMetaInfo(FGuid Guid, const uint16 Index)
{
	TArray<FItemMetaInfo> Items = GetContainerItems(Guid);

	return Items[Index];
}

bool ULootContainerSubsystem::ChangeItem(AActor* Causer, FGuid Guid, const FItemMetaInfo& ItemInfo, const uint16 Index)
{
	if (Causer->HasAuthority())
	{
		Data.UpdateContainer(Guid, ItemInfo, Index);

		Multicast_ChangeItem(Guid, FItemMetaInfo_Net(ItemInfo), Index);
	}
	else
	{
		Server_ChangeItem(Guid, FItemMetaInfo_Net(ItemInfo), Index);
	}

	return true;
}

void ULootContainerSubsystem::SwapItem(AActor* Causer, FGuid Guid, const uint16 Prev, const uint16 Next)
{
	if (Causer->HasAuthority())
	{
		Data.SwapItemInContainer(Guid, Prev, Next);
	}
	else
	{
		Server_SwapItem(Guid, Prev, Next);
	}
}

void ULootContainerSubsystem::Server_CreateLootContainer_Implementation(FGuid NewGuid, uint16 Capacity)
{
	Data.AddContainer(NewGuid, Capacity);
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("흠2"));
	Multicast_CreateLootContainer(NewGuid, Capacity);
}


void ULootContainerSubsystem::Multicast_SwapItem_Implementation(FGuid Guid,
	const uint16 Prev, const uint16 Next)
{
}

void ULootContainerSubsystem::Multicast_CreateLootContainer_Implementation(
	FGuid NewGuid, uint16 Capacity)
{
	Data.AddContainer(NewGuid, Capacity);

	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("%d"),
	 Data.GetItems(NewGuid).Num()));
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

	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("흠2"));

	Multicast_ChangeItem(Guid, ItemInfo, Index);
}

void ULootContainerSubsystem::Multicast_ChangeItem_Implementation(FGuid Guid,
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
