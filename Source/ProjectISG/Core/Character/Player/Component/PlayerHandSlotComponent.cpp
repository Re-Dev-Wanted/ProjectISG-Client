#include "PlayerHandSlotComponent.h"

#include "PlayerInventoryComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/GAS/Common/Object/BaseActor.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"

UPlayerHandSlotComponent::UPlayerHandSlotComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UPlayerHandSlotComponent::OnChange(bool bIsEquippable,
	TSubclassOf<AActor> ActorClass, FName SocketName)
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

	if (!bIsEquippable || !IsValid(ActorClass))
	{
		HeldItem = EmptyItem;
		return;
	}

	AActor* Actor = GetWorld()->SpawnActor(ActorClass);
	HeldItem = Cast<ABaseActor>(Actor);
	
	if (HeldItem && !SocketName.IsNone())
	{
		HeldItem->AttachToComponent(Player->GetMesh(), 
		FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
	}
}

void UPlayerHandSlotComponent::OnAttackAction(AActor* Causer)
{
	if (HeldItem)
	{
		HeldItem->OnAttackAction(Causer);
	}
}

void UPlayerHandSlotComponent::OnInteractAction(AActor* Causer)
{
	if (HeldItem)
	{
		HeldItem->OnInteractAction(Causer);
	}
}

void UPlayerHandSlotComponent::OnTouchAction(AActor* Causer)
{
	if (HeldItem)
	{
		HeldItem->OnTouchAction(Causer);
	}
}

void UPlayerHandSlotComponent::BeginPlay()
{
	Super::BeginPlay();

	if (AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(GetOwner()))
	{
		Player->OnUpdateSelectedItem.AddUObject(this, &UPlayerHandSlotComponent::OnChange);
	}
}

void UPlayerHandSlotComponent::InitializeComponent()
{
	Super::InitializeComponent();

	EmptyItem = NewObject<ABaseActor>(this, TEXT("Empty"));
	HeldItem = EmptyItem;
}

bool UPlayerHandSlotComponent::IsHousingHandItem()
{
	if (!HeldItem || HeldItem == EmptyItem)
	{
		return false;
	}

	if (AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(GetOwner()))
	{
		int Index = Player->GetPlayerInventoryComponent()->GetCurrentSlotIndex();

		const AMainPlayerState* PS = Cast<AMainPlayerState>(Player->GetPlayerState());

		const uint16 ItemId = PS->GetInventoryComponent()->GetInventoryList()[
			Index].GetId();

		return UItemManager::IsItemCanHousing(ItemId);
	}

	return false;
}
