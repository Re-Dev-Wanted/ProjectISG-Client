#include "PlayerHandSlotComponent.h"

#include "PlayerInventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/GAS/Common/Object/BaseActor.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"

UPlayerHandSlotComponent::UPlayerHandSlotComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;

	SetIsReplicatedByDefault(true);
}

void UPlayerHandSlotComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetIsReplicated(true);
}

void UPlayerHandSlotComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (EndPlayReason == EEndPlayReason::Type::Quit)
	{
		if (HeldItem->IsValidLowLevel())
		{
			HeldItem->Destroy();
		}
	}
}

void UPlayerHandSlotComponent::GetLifetimeReplicatedProps(
	TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerHandSlotComponent, HeldItem);
	DOREPLIFETIME(UPlayerHandSlotComponent, ItemId);
}

void UPlayerHandSlotComponent::InitializePlayerHandSlot()
{
	Player = Cast<AMainPlayerCharacter>(
		GetOwner());

	if (!Player)
	{
		return;
	}

	const AMainPlayerState* PS = Player->GetPlayerState<AMainPlayerState>();
	if (PS)
	{
		PS->GetInventoryComponent()
				   ->OnInventoryUpdateNotified.AddDynamic(
					   this, &ThisClass::OnInventoryUpdated);
	}

	OnInventoryUpdated();
}

FString UPlayerHandSlotComponent::GetItemUsingType()
{
	if (!HeldItem)
	{
		return FString();
	}

	return UItemManager::GetItemUsingType(GetItemId());
}

bool UPlayerHandSlotComponent::IsHousingHandItem()
{
	if (!HeldItem)
	{
		return false;
	}

	return UItemManager::IsItemCanHousing(GetItemId());
}

void UPlayerHandSlotComponent::ClearHand()
{
	if (HeldItem)
	{
		ItemId = 0;
		ABaseActor* DestroyActor = HeldItem.Get();
		HeldItem = nullptr;
		DestroyActor->Destroy();
	}
}

void UPlayerHandSlotComponent::ToggleShowItem(const bool IsShow)
{
	if (!HeldItem)
	{
		return;
	}

	HeldItem->SetActorHiddenInGame(!IsShow);
}

void UPlayerHandSlotComponent::Server_ChangeItem_Implementation(uint16 ChangeItemId)
{
	ClearHand();

	const FItemInfoData ItemInfoData = UItemManager::GetItemInfoById(
		ChangeItemId);

	const TSubclassOf<AActor> ActorClass = ItemInfoData.GetShowItemActor();

	const bool bIsStructure = ItemInfoData.GetItemType() !=
		EItemType::Equipment && UItemManager::IsItemCanHousing(ChangeItemId);

	if (!ActorClass || bIsStructure)
	{
		return;
	}

	AActor* Actor = GetWorld()->SpawnActor(ActorClass);
	Actor->SetReplicates(true);
	Actor->SetReplicateMovement(true);
	HeldItem = Cast<ABaseActor>(Actor);

	if (Player->HasAuthority())
	{
		ItemId = ChangeItemId;
		const FName SocketName = UItemManager::GetSocketNameById(ChangeItemId);

		if (!SocketName.IsNone() && Player && Player->GetMesh())
		{
			HeldItem->AttachToComponent(Player->GetMesh(),
										FAttachmentTransformRules::SnapToTargetNotIncludingScale,
										SocketName);
		}
	}
	else
	{
		Multicast_ChangeItem(ChangeItemId, HeldItem);
	}
}

void UPlayerHandSlotComponent::Multicast_ChangeItem_Implementation(uint16
	ChangeItemId, ABaseActor* ChangeHeldItem)
{
	if (ChangeHeldItem)
	{
		const FName SocketName = UItemManager::GetSocketNameById(ChangeItemId);

		if (!SocketName.IsNone() && Player && Player->GetMesh())
		{
			HeldItem->AttachToComponent(Player->GetMesh(),
			                            FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			                            SocketName);
		}
	}
}

void UPlayerHandSlotComponent::OnInventoryUpdated()
{
	const AMainPlayerCharacter* OwnerPlayer = Cast<AMainPlayerCharacter>(
		GetOwner());

	const uint32 CurrentIndex = OwnerPlayer->GetPlayerInventoryComponent()->
	                                         GetCurrentSlotIndex();

	UE_LOG(LogTemp, Display, TEXT("현재 인벤토리 Index: %d"), CurrentIndex);

	const FItemMetaInfo ItemMetaInfo = OwnerPlayer->
	                                   GetPlayerState<AMainPlayerState>()->
	                                   GetInventoryComponent()->
	                                   GetInventoryList()[CurrentIndex];

	Server_ChangeItem(ItemMetaInfo.GetId());
}
