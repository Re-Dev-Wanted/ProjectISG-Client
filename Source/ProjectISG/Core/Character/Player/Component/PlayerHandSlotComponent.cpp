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
		Player->OnInputBindingNotified.AddDynamic(this, &UPlayerHandSlotComponent::BindingInputActions);
		Player->OnUpdateSelectedItem.AddDynamic(this, &UPlayerHandSlotComponent::OnChange);
	}
}

void UPlayerHandSlotComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!Player)
	{
		return;
	}

	if (!IsUseInputAction)
	{
		return;
	}
	
	if (!HeldItem)
	{
		return;
	}

	const AMainPlayerController* PC = Player->GetController<AMainPlayerController>();

	if (!PC)
	{
		return;
	}
	
	if (ABaseInteractiveActor* InteractiveActor = Cast<ABaseInteractiveActor>(HeldItem))
	{
		if (InteractiveActor->GetCanTouch() && PC->GetMainHUD())
		{
			PC->GetMainHUD()->ToggleInteractiveUI(TEXT("RM"), 
			InteractiveActor->GetTouchDisplayText());
		}
	}
}

void UPlayerHandSlotComponent::OnAction()
{
	if (!IsUseInputAction)
	{
		return;
	}
	
	if (!HeldItem)
	{
		return;
	}

	if (ABaseInteractiveActor* InteractiveActor = Cast<ABaseInteractiveActor>(HeldItem))
	{
		InteractiveActor->OnTouch(GetOwner());
	}
}

void UPlayerHandSlotComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerHandSlotComponent, HeldItem);
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
	Actor->SetReplicates(true);
	Actor->SetReplicateMovement(true);
	HeldItem = Cast<ABaseActor>(Actor);
	ItemId = _ItemId;

	const FName SocketName = UItemManager::GetSocketNameById(_ItemId);
	
	if (HeldItem && !SocketName.IsNone())
	{
		HeldItem->AttachToComponent(Player->GetMesh(), 
		FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
	}
}

void UPlayerHandSlotComponent::BindingInputActions(UEnhancedInputComponent* EnhancedInputComponent)
{
	EnhancedInputComponent->BindAction(TouchAction,
									   ETriggerEvent::Triggered, this,
									   &UPlayerHandSlotComponent::OnAction);
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

void UPlayerHandSlotComponent::SetIsUseInputAction(const bool NewIsUseInputAction)
{
	IsUseInputAction = NewIsUseInputAction;
}
