﻿#include "PlayerInventoryComponent.h"
#include "EnhancedInputComponent.h"
#include "Net/UnrealNetwork.h"

#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Core/UI/Base/Components/UIManageComponent.h"
#include "ProjectISG/Core/UI/Gameplay/MainHUD/UI/UIC_MainHUD.h"
#include "ProjectISG/Core/UI/HUD/Inventory/InventoryList.h"
#include "ProjectISG/Core/UI/Popup/Inventory/UI/UIC_InventoryUI.h"
#include "ProjectISG/Core/UI/Popup/Trading/UI/UIC_TradingUI.h"
#include "ProjectISG/Core/UI/Popup/Trading/UI/UIV_TradingUI.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"

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
	TArray<FLifetimeProperty>& OutLifetimeProps) const
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
		ChangeCurrentSlotIndex(MoveToSlot + MaxMainSlotIndex + 1);
		return;
	}

	if (MoveToSlot > MaxMainSlotIndex)
	{
		ChangeCurrentSlotIndex(MoveToSlot - MaxMainSlotIndex - 1);
		return;
	}

	ChangeCurrentSlotIndex(MoveToSlot);
}

void UPlayerInventoryComponent::ChangeCurrentSlotIndex(const uint8 NewIndex)
{
	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(GetOwner());

	const AMainPlayerState* PS = Player->GetPlayerState<AMainPlayerState>();

	const FItemMetaInfo ItemMetaInfo = PS->GetInventoryComponent()->
	                                       GetInventoryList()[NewIndex];

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

	CurrentSlotIndex = NewIndex;
	Server_ChangeCurrentSlotIndex(NewIndex);

	PS->GetInventoryComponent()->OnInventoryUpdateNotified.Broadcast();
}

bool UPlayerInventoryComponent::RemoveItemCurrentSlotIndex(const int32 Count)
{
	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(GetOwner());

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
