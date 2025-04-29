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

	EmptyItem = NewObject<ABaseActor>(this, TEXT("Empty"));
	HeldItem = EmptyItem;

	if (AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(GetOwner()))
	{
		Player->OnUpdateSelectedItem.AddDynamic(this, &UPlayerHandSlotComponent::OnChange);
	}
}

void UPlayerHandSlotComponent::OnChange(TSubclassOf<AActor> ActorClass, FItemMetaInfo _ItemMetaInfo)
{
	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(GetOwner());

	if (!Player)
	{
		return;
	}

	if (HeldItem)
	{
		if (HeldItem->IsAttachedTo(Player))
		{
			HeldItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		}

		if (HeldItem != EmptyItem)
		{
			HeldItem->Destroy();
		}
	}

	const FItemInfoData ItemInfoData = UItemManager::GetItemInfoById(ItemMetaInfo.GetId());

	const bool bIsStructure = ItemInfoData.GetItemType() != EItemType::Equipment && UItemManager::IsItemCanHousing(ItemMetaInfo.GetId());

	if (bIsStructure || !IsValid(ActorClass))
	{
		HeldItem = EmptyItem;
		ItemMetaInfo = FItemMetaInfo();
		return;
	}

	AActor* Actor = GetWorld()->SpawnActor(ActorClass);
	HeldItem = Cast<ABaseActor>(Actor);
	ItemMetaInfo = _ItemMetaInfo;

	const FName SocketName = UItemManager::GetSocketNameById(ItemMetaInfo.GetId());
	
	if (HeldItem && !SocketName.IsNone())
	{
		HeldItem->AttachToComponent(Player->GetMesh(), 
		FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
	}
}

bool UPlayerHandSlotComponent::IsHousingHandItem()
{
	if (!HeldItem || HeldItem == EmptyItem)
	{
		return false;
	}

	if (AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(GetOwner()))
	{
		return UItemManager::IsItemCanHousing(ItemMetaInfo.GetId());
	}

	return false;
}
