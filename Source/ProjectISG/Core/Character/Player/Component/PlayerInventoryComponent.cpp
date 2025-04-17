#include "PlayerInventoryComponent.h"

#include "EnhancedInputComponent.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Core/UI/HUD/MainHUD.h"
#include "ProjectISG/Core/UI/HUD/Inventory/InventoryList.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"


UPlayerInventoryComponent::UPlayerInventoryComponent()
{
	bWantsInitializeComponent = true;
}

void UPlayerInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPlayerInventoryComponent::InitializeComponent()
{
	Super::InitializeComponent();

	AMainPlayerCharacter* OwnerPlayer = Cast<AMainPlayerCharacter>(GetOwner());
	OwnerPlayer->OnInputBindingNotified.AddDynamic(
		this, &ThisClass::BindingInputActions);
}

void UPlayerInventoryComponent::BindingInputActions(
	UEnhancedInputComponent* EnhancedInputComponent)
{
	EnhancedInputComponent->BindAction(ToggleInventoryInputAction,
	                                   ETriggerEvent::Triggered, this,
	                                   &ThisClass::ToggleInventory);
	EnhancedInputComponent->BindAction(SelectHotSlotInputAction,
	                                   ETriggerEvent::Triggered, this,
	                                   &ThisClass::SelectHotSlot);
	EnhancedInputComponent->BindAction(MoveToHotSlotInputAction,
	                                   ETriggerEvent::Triggered, this,
	                                   &ThisClass::MoveHotSlot);
}

void UPlayerInventoryComponent::ToggleInventory()
{
	AMainPlayerController* PC = Cast<AMainPlayerController>(
		GetOwner()->GetInstigatorController());

	IsOpenedInventory = !IsOpenedInventory;
	PC->ToggleInventoryUI(IsOpenedInventory);
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

	const uint16 ItemId = PS->GetInventoryComponent()->GetInventoryList()[NewIndex].GetId();

	const FItemInfoData ItemInfoData = UItemManager::GetItemInfoById(ItemId);

	const bool bAvailable = UItemManager::IsItemCanHousing(ItemId);

	if (bAvailable)
	{
		Player->GetPlacementIndicatorComponent()->OnActivate(ItemInfoData.GetShowItemActor());
		Player->SetMainHandItem(nullptr);
	}
	else
	{
		Player->GetPlacementIndicatorComponent()->OnDeactivate();
		AActor* SpawnActor = GetWorld()->SpawnActor(ItemInfoData.GetShowItemActor());
		Player->SetMainHandItem(SpawnActor);
	}

	const AMainPlayerController* PC = Cast<AMainPlayerController>(
		GetOwner()->GetInstigatorController());

	if (!PC)
	{
		return;
	}

	PC->GetMainHUD()->GetMainSlotList()->SelectSlot(
		CurrentSlotIndex, NewIndex);

	CurrentSlotIndex = NewIndex;
}
