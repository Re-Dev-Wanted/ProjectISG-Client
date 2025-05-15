#include "LootContainerSubsystem.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

ULootContainerSubsystem::ULootContainerSubsystem()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
    
	SetIsReplicatedByDefault(true);
    
	// Initialize the FastArraySerializer
	Data.OwningObject = this;
}

void ULootContainerSubsystem::InitializeComponent()
{
	Super::InitializeComponent();
}

void ULootContainerSubsystem::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ULootContainerSubsystem, Data);
}

bool ULootContainerSubsystem::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch,
	FReplicationFlags* RepFlags)
{
	return Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
}

void ULootContainerSubsystem::LoadAllDataAsync()
{
}

void ULootContainerSubsystem::CreateLootContainer(AActor* Causer, FGuid NewGuid, 
int32 Capacity)
{
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("%s"), *NewGuid.ToString()));
	
	if (Causer->HasAuthority())
	{
		Data.AddContainer(NewGuid, Capacity);
	}
	else
	{
		Server_CreateLootContainer(NewGuid, Capacity);
	}
}

bool ULootContainerSubsystem::GetContainerItems(FGuid Guid, TArray<FItemMetaInfo>& OutItems)
{
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("[%s] %d"),
	 *Guid.ToString(), Data.GetItems(Guid).Num()));

	if (Data.Contains(Guid))
	{
		OutItems = Data.GetItems(Guid);

		return true;
	}

	return false;
}

FItemMetaInfo ULootContainerSubsystem::GetItemMetaInfo(FGuid Guid, const uint16 Index)
{
	TArray<FItemMetaInfo> Items;

	GetContainerItems(Guid, Items);

	return Items[Index];
}

bool ULootContainerSubsystem::ChangeItem(AActor* Causer, FGuid Guid, const FItemMetaInfo& ItemInfo, const uint16 Index)
{
	if (Causer->HasAuthority())
	{
		Data.UpdateContainer(Guid, ItemInfo, Index);
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
}
