#include "PlayerHandSlotComponent.h"

#include "EnhancedInputComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/UI/Gameplay/MainHUD/UI/UIC_MainHUD.h"
#include "ProjectISG/GAS/Common/Object/BaseActor.h"
#include "ProjectISG/GAS/Common/Object/BaseInteractiveActor.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"

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
		Player->OnUpdateSelectedItem.AddDynamic(this, &UPlayerHandSlotComponent::OnChange);
	}
}

void UPlayerHandSlotComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerHandSlotComponent, HeldItem);
	DOREPLIFETIME(UPlayerHandSlotComponent, ItemId);
}

void UPlayerHandSlotComponent::OnChange(uint16 _ItemId)
{
	if (!Player)
	{
		return;
	}

	if (HeldItem)
	{
		ABaseActor* DestroyActor = HeldItem.Get();
		HeldItem = nullptr;
		DestroyActor->Destroy();
		//ItemId = 0;
		Server_ChangeItemId(0);
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
	Actor->SetReplicates(true);
	Actor->SetReplicateMovement(true);
	HeldItem = Cast<ABaseActor>(Actor);
	//ItemId = _ItemId;
	Server_ChangeItemId(_ItemId);

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

void UPlayerHandSlotComponent::Server_ChangeItemId_Implementation(
	uint16 ChangeItemId)
{
	NetMulticast_ChangeItemId(ChangeItemId);
}

void UPlayerHandSlotComponent::NetMulticast_ChangeItemId_Implementation(
	uint16 ChangeItemId)
{
	ItemId = ChangeItemId;
}
