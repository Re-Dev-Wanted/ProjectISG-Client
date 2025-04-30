#include "PlayerHandSlotComponent.h"

#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/GAS/Common/Object/BaseActor.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"

UPlayerHandSlotComponent::UPlayerHandSlotComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}

void UPlayerHandSlotComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(GetOwner()))
	{
		Player->OnUpdateSelectedItem.AddDynamic(this, &UPlayerHandSlotComponent::OnChange);
	}
}

void UPlayerHandSlotComponent::OnChange(uint16 _ItemId)
{
	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(GetOwner());

	if (!Player)
	{
		return;
	}

	if (HeldItem)
	{
		ABaseActor* DestroyActor = HeldItem.Get();
		HeldItem = nullptr;
		DestroyActor->Destroy();
		ItemId = 0;
	}

	const FItemInfoData ItemInfoData = UItemManager::GetItemInfoById(_ItemId);

	const TSubclassOf<AActor> ActorClass = ItemInfoData.GetShowItemActor();

	const bool bIsStructure = ItemInfoData.GetItemType() != 
	EItemType::Equipment && UItemManager::IsItemCanHousing(_ItemId);

	if (!ActorClass || bIsStructure)
	{
		return;
	}

	AActor* Actor = GetWorld()->SpawnActor(ActorClass);
	HeldItem = Cast<ABaseActor>(Actor);
	ItemId = _ItemId;

	const FName SocketName = UItemManager::GetSocketNameById(_ItemId);
	
	if (HeldItem && !SocketName.IsNone())
	{
		HeldItem->AttachToComponent(Player->GetMesh(), 
		FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
	}
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
