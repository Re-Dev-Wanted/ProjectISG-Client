// Fill out your copyright notice in the Description page of Project Settings.


#include "TicketMachine.h"

#include "Components/BoxComponent.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Controller/TutorialPlayerController.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"
#include "ProjectISG/Systems/QuestStory/Component/QuestManageComponent.h"


ATicketMachine::ATicketMachine()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Root->SetBoxExtent(FVector(52.5f, 32.f, 100.f));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	Mesh->SetRelativeLocation(FVector(-45.f, -30.f, -100.f));

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(
		TEXT("'/Game/TrainStation/Meshes/SM_TicketMachine.SM_TicketMachine'"));
	if (tempMesh.Succeeded())
	{
		Mesh->SetStaticMesh(tempMesh.Object);
	}

	ConstructorHelpers::FObjectFinder<UMaterialInstance> tempMat(
		TEXT(
			"'/Game/TrainStation/Materials/Mi_TicketMachine.Mi_TicketMachine'"));
	if (tempMat.Succeeded())
	{
		Mesh->SetMaterial(0, tempMat.Object);
	}
}

void ATicketMachine::BeginPlay()
{
	Super::BeginPlay();

	SetQuestInteractiveActorOverlayMaterial(false);
}

void ATicketMachine::OnInteractive(AActor* Causer)
{
	Super::OnInteractive(Causer);

	AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(Causer);
	if (Player)
	{
		bIsBoughtTicket = true;
		Player->GetPlayerState<AMainPlayerState>()->GetInventoryComponent()->
		        AddItem(UItemManager::GetInitialItemMetaDataById(26));

		ATutorialPlayerController* TutorialPlayerController = Player->
			GetController<ATutorialPlayerController>();
		TutorialPlayerController->GetQuestManageComponent()->EndQuest(true);
		TutorialPlayerController->StartQuest(NextQuest);
	}
}

bool ATicketMachine::GetCanInteractive() const
{
	return !bIsBoughtTicket;
}

FString ATicketMachine::GetInteractiveDisplayText() const
{
	return TEXT("티켓구매");
}

void ATicketMachine::SetQuestInteractiveActorOverlayMaterial(bool value)
{
	if (!value)
	{
		Mesh->SetOverlayMaterial(QuestInteractMaterial);
	}
	else
	{
		Mesh->SetOverlayMaterial(nullptr);
	}
}
