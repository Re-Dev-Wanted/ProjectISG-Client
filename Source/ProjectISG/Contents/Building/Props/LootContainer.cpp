#include "LootContainer.h"

#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/InteractionComponent.h"
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
	TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALootContainer, Items);
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

		SetInteractingPlayer(Player);
		
		if (HasAuthority())
		{
			SetOwner(PC);
		}
		else
		{
			PC->Server_SetOwnerActor(this);

			Player->GetInteractionComponent()->Server_OnInteractiveResponse(Causer);
		}
		
		
		if (Player->IsLocallyControlled())
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), SFX_Open, GetActorLocation(), 1, 1, 0.5f);
			
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

			UIController->SetContainer(OutDatas, this, this);
		}
	}
}

void ALootContainer::OnInteractiveResponse(AActor* Causer)
{
	Super::OnInteractiveResponse(Causer);

	AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(Causer);
	Server_SetInteractingPlayer(Player);
}

bool ALootContainer::GetCanInteractive() const
{
	return !IsValid(GetInteractingPlayer());
}

bool ALootContainer::GetCanTouch() const
{
	return false;
}

FString ALootContainer::GetInteractiveDisplayText() const
{
	return TEXT("열기");
}

bool ALootContainer::ChangeItem(AActor* Causer, const FItemMetaInfo& ItemInfo,
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

void ALootContainer::OnClosed()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), SFX_Close, GetActorLocation());
	
	if (!IsValid(GetInteractingPlayer()))
	{
		return;
	}
	
	AMainPlayerController* PC = Cast<AMainPlayerController>(
		GetInteractingPlayer()->GetController());

	if (!PC)
	{
		return;
	}
	
	if (!HasAuthority())
	{
		GetInteractingPlayer()->GetInteractionComponent()->Server_OnInteractiveResponse
		(nullptr);
	}

	SetInteractingPlayer(nullptr);
	
}

void ALootContainer::UpdateItem(int32 Index, const FItemMetaInfo& NewItem)
{
	if (Items.IsValidIndex(Index))
	{
		Items[Index] = NewItem;
		
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
