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
}

void UPlayerHandSlotComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetIsReplicated(true);

	Player = Cast<AMainPlayerCharacter>(GetOwner());

	if (Player)
	{
		Player->OnUpdateSelectedItem.AddDynamic(
			this, &UPlayerHandSlotComponent::OnChange);
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
	const AMainPlayerCharacter* OwnerPlayer = Cast<AMainPlayerCharacter>(
		GetOwner());

	OwnerPlayer->GetPlayerState<AMainPlayerState>()->GetInventoryComponent()
	           ->OnInventoryUpdateNotified.AddDynamic(
		           this, &ThisClass::OnChangeHandItemSlot);
}

void UPlayerHandSlotComponent::OnChange(uint16 _ItemId)
{
	if (!Player)
	{
		return;
	}

	Server_ChangeItemId(_ItemId);
}

FString UPlayerHandSlotComponent::GetItemUsingType()
{
	if (!HeldItem)
	{
		return FString();
	}

	return UItemManager::GetItemUsingType(ItemId);
}

bool UPlayerHandSlotComponent::IsHousingHandItem()
{
	if (!HeldItem)
	{
		return false;
	}

	return UItemManager::IsItemCanHousing(ItemId);
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

void UPlayerHandSlotComponent::Server_ChangeItemId_Implementation(
	uint16 ChangeItemId)
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

	if (GetOwner()->HasAuthority())
	{
		ItemId = ChangeItemId;

		if (HeldItem)
		{
			const FName SocketName = UItemManager::GetSocketNameById(ItemId);

			if (!SocketName.IsNone() && Player && Player->GetMesh())
			{
				HeldItem->AttachToComponent(Player->GetMesh(),
				                            FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				                            SocketName);
			}
		}
	}
	{
		Multicast_ChangeItemId(ChangeItemId, HeldItem);
	}
}

void UPlayerHandSlotComponent::Multicast_ChangeItemId_Implementation(uint16
	ChangeItemId, class ABaseActor* ChangeHeldItem)
{
	ItemId = ChangeItemId;

	if (ChangeHeldItem)
	{
		const FName SocketName = UItemManager::GetSocketNameById(ItemId);

		if (!SocketName.IsNone() && Player && Player->GetMesh())
		{
			HeldItem->AttachToComponent(Player->GetMesh(),
			                            FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			                            SocketName);
		}
	}
}

void UPlayerHandSlotComponent::OnChangeHandItemSlot()
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

	OnChange(ItemMetaInfo.GetId());
}
