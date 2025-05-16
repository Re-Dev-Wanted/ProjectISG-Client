#include "LootContainer.h"

#include "Net/UnrealNetwork.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/UI/Base/Components/UIManageComponent.h"
#include "ProjectISG/Core/UI/Popup/LootContainer/UI/UIC_LootContainerUI.h"

void ALootContainer::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		Items.Init(FItemMetaInfo(), Capacity);
	}
}

void ALootContainer::GetLifetimeReplicatedProps(
	TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALootContainer, Items);
}

void ALootContainer::SetOption(bool bIsGhost, bool bIsBlock)
{
	Super::SetOption(bIsGhost, bIsBlock);
}

void ALootContainer::OnInteractive(AActor* Causer)
{
	Super::OnInteractive(Causer);

	if (AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(Causer))
	{
		AMainPlayerController* PC = Cast<AMainPlayerController>(
			Player->GetController());

		if (!PC)
		{
			return;
		}

		if (HasAuthority())
		{
			SetOwner(PC);
		}
		else
		{
			PC->Server_SetOwnerActor(this);
		}

		if (Player->IsLocallyControlled())
		{
			PC->PushUI(EUIName::Popup_LootContainerUI);

			UUIC_LootContainerUI* UIController = Cast<UUIC_LootContainerUI>
			(PC->GetUIManageComponent()->ControllerInstances[
				EUIName::Popup_LootContainerUI]);

			TArray<FItemMetaInfo> OutDatas;
			Algo::Transform(Items,
				OutDatas,
				[](const FItemMetaInfo_Net& Data)
				{
					FItemMetaInfo OutInfo;
					Data.To(OutInfo);

					return OutInfo;
				}
			);

			UIController->SetContainer(FGuid(), OutDatas, this);
		}
	}
}

void ALootContainer::OnInteractiveResponse(AActor* Causer)
{
	Super::OnInteractiveResponse(Causer);

	AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(Causer);
	OnRep_SetInteractingPlayer(Player);
}

bool ALootContainer::GetCanInteractive() const
{
	return !IsValid(GetInteractingPlayer());
}

FString ALootContainer::GetInteractiveDisplayText() const
{
	return TEXT("열기");
}

bool ALootContainer::ChangeItem(AActor* Causer,                                 const FItemMetaInfo& ItemInfo,
                                const uint16 Index)
{
	if (!Items.IsValidIndex(Index))
	{
		return false;
	}

	if (HasAuthority())
	{
		UpdateItem(Index, ItemInfo);
	}
	else
	{
		Server_UpdateItem(Index, FItemMetaInfo_Net(ItemInfo));
	}

	return true;
}

void ALootContainer::SwapItem(AActor* Causer, const uint16 Prev,
                              const uint16 Next)
{
	if (!Items.IsValidIndex(Prev) || !Items.IsValidIndex(Next))
	{
		return;
	}

	if (HasAuthority())
	{
		SwapItemInternal(Prev, Next);
	}
	else
	{
		Server_SwapItem(Prev, Next);
	}
}

void ALootContainer::SwapItemInternal(uint16 PrevIndex, uint16 NextIndex)
{
	if (Items.IsValidIndex(PrevIndex) && Items.IsValidIndex(NextIndex))
	{
		const FItemMetaInfo_Net Temp = Items[PrevIndex];
		Items[PrevIndex] = Items[NextIndex];
		Items[NextIndex] = Temp;

		NetMulticast_NotifyItemsChanged();

		ForceNetUpdate();
	}
}

void ALootContainer::Server_SwapItem_Implementation(
	const uint16 Prev, const uint16 Next)
{
	SwapItemInternal(Prev, Next);
}

FItemMetaInfo ALootContainer::GetItemMetaInfo(const uint16 Index)
{
	if (!Items.IsValidIndex(Index))
	{
		return FItemMetaInfo();
	}

	FItemMetaInfo OutInfo;
	Items[Index].To(OutInfo);

	return OutInfo;
}

void ALootContainer::OnRep_Items()
{
}

void ALootContainer::UpdateItem(int32 Index, const FItemMetaInfo& NewItem)
{
	if (Items.IsValidIndex(Index))
	{
		Items[Index] = NewItem;

		NetMulticast_NotifyItemsChanged();

		ForceNetUpdate();
	}
}

void ALootContainer::Server_UpdateItem_Implementation(
	int32 Index, const FItemMetaInfo_Net& NewItem)
{
	FItemMetaInfo ItemInfo;
	NewItem.To(ItemInfo);

	UpdateItem(Index, ItemInfo);
}

void ALootContainer::NetMulticast_NotifyItemsChanged_Implementation()
{
	if (!HasAuthority())
	{
		OnRep_Items();
	}
}
