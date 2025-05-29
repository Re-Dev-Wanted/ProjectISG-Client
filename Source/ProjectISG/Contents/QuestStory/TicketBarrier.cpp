// Fill out your copyright notice in the Description page of Project Settings.


#include "TicketBarrier.h"

#include "Components/BoxComponent.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"


class AMainPlayerState;

ATicketBarrier::ATicketBarrier()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Root->SetBoxExtent(FVector(32.f,450.f,100.f));
	
	Mesh0 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh0"));
	Mesh0->SetupAttachment(Root);
	Mesh0->SetRelativeLocation(FVector(0.f, 0.f, -50.f));
	
	Mesh1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh1"));
	Mesh1->SetupAttachment(Root);
	Mesh1->SetRelativeLocation(FVector(0.f, 140.f, -50.f));

	Mesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh2"));
	Mesh2->SetupAttachment(Root);
	Mesh2->SetRelativeLocation(FVector(0.f, 170.f, -50.f));

	Mesh3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh3"));
	Mesh3->SetupAttachment(Root);
	Mesh3->SetRelativeLocation(FVector(0.f, 270.f, -50.f));


	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh (TEXT("'/Game/TrainStation/Meshes/SM_TicketBarrier_Open.SM_TicketBarrier_Open'"));
	if (tempMesh.Succeeded())
	{
		Mesh0->SetStaticMesh(tempMesh.Object);
		Mesh1->SetStaticMesh(tempMesh.Object);
		Mesh2->SetStaticMesh(tempMesh.Object);
		Mesh3->SetStaticMesh(tempMesh.Object);
	}

	ConstructorHelpers::FObjectFinder<UMaterialInstance> tempMat (TEXT("'/Game/TrainStation/Materials/Mi_TicketBarrier.Mi_TicketBarrier'"));
	if (tempMat.Succeeded())
	{
		Mesh0->SetMaterial(0, tempMat.Object);
		Mesh1->SetMaterial(0, tempMat.Object);
		Mesh2->SetMaterial(0, tempMat.Object);
		Mesh3->SetMaterial(0, tempMat.Object);
	}
}

void ATicketBarrier::BeginPlay()
{
	Super::BeginPlay();
	DisplayText = TEXT("티켓을 구매 후, F키를 눌러주세요");
	SetQuestInteractiveActorOverlayMaterial(true);
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
			SetQuestInteractiveActorOverlayMaterial(true);
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

void ATicketBarrier::SetQuestInteractiveActorOverlayMaterial(bool value)
{
	if (!value)
	{
		Mesh0->SetOverlayMaterial(QuestInteractMaterial);
		Mesh1->SetOverlayMaterial(QuestInteractMaterial);
		Mesh2->SetOverlayMaterial(QuestInteractMaterial);
		Mesh3->SetOverlayMaterial(QuestInteractMaterial);
		SetVisibilityBillboard(true);
	}
	else
	{
		Mesh0->SetOverlayMaterial(nullptr);
		Mesh1->SetOverlayMaterial(nullptr);
		Mesh2->SetOverlayMaterial(nullptr);
		Mesh3->SetOverlayMaterial(nullptr);
		SetVisibilityBillboard(false);
	}
}
