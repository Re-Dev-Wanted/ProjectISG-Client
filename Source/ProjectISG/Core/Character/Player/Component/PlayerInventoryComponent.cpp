#include "PlayerInventoryComponent.h"
#include "EnhancedInputComponent.h"
#include "Net/UnrealNetwork.h"

#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Core/UI/Base/Components/UIManageComponent.h"
#include "ProjectISG/Core/UI/Gameplay/MainHUD/UI/UIC_MainHUD.h"
#include "ProjectISG/Core/UI/Popup/Inventory/UI/UIC_InventoryUI.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"

UPlayerInventoryComponent::UPlayerInventoryComponent()
{
	bWantsInitializeComponent = true;
}

void UPlayerInventoryComponent::InitializeComponent()
{
	AMainPlayerCharacter* OwnerPlayer = Cast<AMainPlayerCharacter>(GetOwner());

	OwnerPlayer->OnInputBindingNotified.AddDynamic(
		this, &ThisClass::BindingInputActions);
}

void UPlayerInventoryComponent::GetLifetimeReplicatedProps(
	TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, CurrentSlotIndex);
}

void UPlayerInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPlayerInventoryComponent::InitializePlayerInventory()
{
	const AMainPlayerCharacter* OwnerPlayer = Cast<AMainPlayerCharacter>(
		GetOwner());

	OwnerPlayer->GetPlayerState<AMainPlayerState>()->GetInventoryComponent()
	           ->OnInventoryUpdateNotified.AddDynamic(
		           this, &ThisClass::UpdatePlayerInventoryUI);
}

void UPlayerInventoryComponent::BindingInputActions(
	UEnhancedInputComponent* EnhancedInputComponent)
{
	EnhancedInputComponent->BindAction(OpenInventoryInputAction,
	                                   ETriggerEvent::Triggered, this,
	                                   &ThisClass::OpenInventory);
	EnhancedInputComponent->BindAction(SelectHotSlotInputAction,
	                                   ETriggerEvent::Triggered, this,
	                                   &ThisClass::SelectHotSlot);
	EnhancedInputComponent->BindAction(MoveToHotSlotInputAction,
	                                   ETriggerEvent::Triggered, this,
	                                   &ThisClass::MoveHotSlot);
}

void UPlayerInventoryComponent::OpenInventory()
{
	AMainPlayerController* PC = Cast<AMainPlayerController>(
		GetOwner()->GetInstigatorController());

	PC->OpenInventory();
}

void UPlayerInventoryComponent::SelectHotSlot(const FInputActionValue& Value)
{
	const float HotSlotKey = Value.Get<float>() - 1;
	ChangeCurrentSlotIndex(static_cast<uint8>(HotSlotKey));
}

void UPlayerInventoryComponent::MoveHotSlot(const FInputActionValue& Value)
{
	const int MoveToSlot = CurrentSlotIndex + static_cast<int>(Value.Get<
		float>());

	if (MoveToSlot < 0)
	{
		ChangeCurrentSlotIndex(MoveToSlot + MaxMainSlotIndex);
		return;
	}

	if (MoveToSlot > MaxMainSlotIndex)
	{
		ChangeCurrentSlotIndex(MoveToSlot - MaxMainSlotIndex);
		return;
	}

	ChangeCurrentSlotIndex(MoveToSlot);
}

void UPlayerInventoryComponent::ChangeCurrentSlotIndex(const uint8 NewIndex)
{
	AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(GetOwner());

	const AMainPlayerState* PS = Player->GetPlayerState<AMainPlayerState>();

	const uint16 ItemId = PS->GetInventoryComponent()->GetInventoryList()[
		NewIndex].GetId();

	const FItemInfoData ItemInfoData = UItemManager::GetItemInfoById(ItemId);

	const bool bAvailable = UItemManager::IsItemCanHousing(ItemId);

	if (bAvailable)
	{
		Player->GetPlacementIndicatorComponent()->OnActivate(
			ItemInfoData.GetShowItemActor());
	}
	else
	{
		Player->GetPlacementIndicatorComponent()->OnDeactivate();
	}

	const AMainPlayerController* PC = Cast<AMainPlayerController>(
		GetOwner()->GetInstigatorController());

	if (!PC)
	{
		return;
	}

	if (!PC->GetMainHUD())
	{
		return;
	}

	PC->GetMainHUD()->SelectSlot(CurrentSlotIndex, NewIndex);

	Server_ChangeCurrentSlotIndex(NewIndex);
}

bool UPlayerInventoryComponent::RemoveItemCurrentSlotIndex(const int32 Count)
{
	AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(GetOwner());

	const AMainPlayerState* PS = Player->GetPlayerState<AMainPlayerState>();

	const TObjectPtr<UInventoryComponent> InventoryComponent = PS->
		GetInventoryComponent();

	return InventoryComponent->DropItem(CurrentSlotIndex, Count);
}

void UPlayerInventoryComponent::UpdatePlayerInventoryUI()
{
	const AMainPlayerCharacter* OwnerPlayer = Cast<AMainPlayerCharacter>(
		GetOwner());

	const AMainPlayerController* PC = OwnerPlayer->GetController<
		AMainPlayerController>();

	if (!PC)
	{
		return;
	}

	if (PC->GetMainHUD())
	{
		PC->GetMainHUD()->
		    UpdateMainHotSlot();
	}

	if (PC->GetInventoryUI())
	{
		PC->GetInventoryUI()->
		    UpdateMainSlotItemData();
		OwnerPlayer->GetController<AMainPlayerController>()->GetInventoryUI()->
		             UpdateInventorySlotItemData();
	}
}

void UPlayerInventoryComponent::
Server_ChangeCurrentSlotIndex_Implementation(int idx)
{
	CurrentSlotIndex = idx;
}

