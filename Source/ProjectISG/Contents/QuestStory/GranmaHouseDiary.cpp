// Fill out your copyright notice in the Description page of Project Settings.


#include "GranmaHouseDiary.h"

#include "Components/BoxComponent.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/UI/Base/Components/UIManageComponent.h"


AGranmaHouseDiary::AGranmaHouseDiary()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Root->SetBoxExtent(FVector(50.0f,100.0f,50.0f));

	Book = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Book"));
	Book->SetupAttachment(Root);
	Book->SetRelativeLocation(FVector(30.000000,0.f,40.000000));
	Book->SetRelativeRotation(FRotator(0.f,-180.f,0.f));
	Book->SetRelativeScale3D(FVector(2.5f));

	Desk = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Desk"));
	Desk->SetupAttachment(Root);
	Desk->SetRelativeLocation(FVector(-30.000000,-60.f,-40.000000));
	Desk->SetRelativeScale3D(FVector(1.f));

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempDeskMesh (TEXT("'/Game/VillageHousePack_v1/Meshes/InteriorProps/SM_Table_1.SM_Table_1'"));
	if (tempDeskMesh.Succeeded())
	{
		Desk->SetStaticMesh(tempDeskMesh.Object);
	}

	ConstructorHelpers::FObjectFinder<UMaterialInstance> tempDeskMat0 (TEXT("'/Game/VillageHousePack_v1/Materials/InteriorPropsMaterial/MI_KitchenFurniture_2.MI_KitchenFurniture_2'"));
	ConstructorHelpers::FObjectFinder<UMaterial> tempDeskMat1 (TEXT("'/Game/VillageHousePack_v1/Materials/LODPlaneMaterial/M_LOD.M_LOD'"));
	if (tempDeskMat0.Succeeded() && tempDeskMat1.Succeeded())
	{
		Desk->SetMaterial(0, tempDeskMat0.Object);
		Desk->SetMaterial(1, tempDeskMat1.Object);
	}

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempBookMesh (TEXT("'/Game/VillageHousePack_v1/Meshes/InteriorProps/SM_Book.SM_Book'"));
	if (tempBookMesh.Succeeded())
	{
		Book->SetStaticMesh(tempBookMesh.Object);
	}

	ConstructorHelpers::FObjectFinder<UMaterialInstance> tempBookMat0 (TEXT("'/Game/VillageHousePack_v1/Materials/InteriorPropsMaterial/MI_SmallProps_1.MI_SmallProps_1'"));
	ConstructorHelpers::FObjectFinder<UMaterial> tempBookMat1 (TEXT("'/Game/VillageHousePack_v1/Materials/LODPlaneMaterial/M_LOD.M_LOD'"));
	if (tempBookMat0.Succeeded() && tempBookMat1.Succeeded())
	{
		Book->SetMaterial(0, tempBookMat0.Object);
		Book->SetMaterial(1, tempBookMat1.Object);
	}
}

void AGranmaHouseDiary::BeginPlay()
{
	Super::BeginPlay();

	SetQuestInteractiveActorOverlayMaterial(false);
}

void AGranmaHouseDiary::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGranmaHouseDiary::OnInteractive(AActor* Causer)
{
	Super::OnInteractive(Causer);

	AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(Causer);
	if (Player)
	{
		AMainPlayerController* PC = Cast<AMainPlayerController>(
			Player->GetController());
		if (PC)
		{
			PC->PushUI(EUIName::Popup_MultiPlay);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("메인레벨 이동"));
}

FString AGranmaHouseDiary::GetInteractiveDisplayText() const
{
	return TEXT("일기장 펼치기");
}

bool AGranmaHouseDiary::GetCanInteractive() const
{
	return true;
}

void AGranmaHouseDiary::SetQuestInteractiveActorOverlayMaterial(bool value)
{
	if (!value)
	{
		Book->SetOverlayMaterial(QuestInteractMaterial);
		Desk->SetOverlayMaterial(QuestInteractMaterial);
	}
	else
	{
		Book->SetOverlayMaterial(nullptr);
		Desk->SetOverlayMaterial(nullptr);
	}
}
