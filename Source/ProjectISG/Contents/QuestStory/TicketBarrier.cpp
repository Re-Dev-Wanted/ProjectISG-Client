// Fill out your copyright notice in the Description page of Project Settings.


#include "TicketBarrier.h"

#include "Components/BoxComponent.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/PlayerHandSlotComponent.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"


class AMainPlayerState;

ATicketBarrier::ATicketBarrier()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	SetRootComponent(Root);
}

void ATicketBarrier::BeginPlay()
{
	Super::BeginPlay();
	DisplayText = TEXT("티켓을 구매 후, F키를 눌러주세요");
}

void ATicketBarrier::OnInteractive(AActor* Causer)
{
	Super::OnInteractive(Causer);

	AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(Causer);
	AMainPlayerState* PS = Player->GetPlayerState<AMainPlayerState>(); 
	if (Player && PS)
	{
		if (PS->GetInventoryComponent()->HasItemInInventory(26, 1))
		{
			Root->SetCollisionProfileName(TEXT("NoCollision"));
			PS->GetInventoryComponent()->RemoveItem(26, 1);
		}
	}
}

bool ATicketBarrier::GetCanInteractive() const
{
	return !bPaidTicket;
}

FString ATicketBarrier::GetInteractiveDisplayText() const
{
	return DisplayText;
}
