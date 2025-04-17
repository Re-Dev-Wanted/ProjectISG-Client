// Fill out your copyright notice in the Description page of Project Settings.


#include "Bed.h"

#include "TimeManager.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"

// Sets default values
ABed::ABed()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	Root = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Root->SetBoxExtent(FVector(110.f, 55.f, 32.f));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	Mesh->SetRelativeScale3D(FVector(2.f, 1.f, 0.5f));
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(
		TEXT("'/Engine/BasicShapes/Cube.Cube'"));
	if (tempMesh.Succeeded())
	{
		Mesh->SetStaticMesh(tempMesh.Object);
	}

	ConstructorHelpers::FObjectFinder<UMaterial> tempMat(
		TEXT("'/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial'"));
	if (tempMat.Succeeded())
	{
		Mesh->SetMaterial(0, tempMat.Object);
	}

	Root->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void ABed::BeginPlay()
{
	Super::BeginPlay();


	TimeManager = Cast<ATimeManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(),
		                                  ATimeManager::StaticClass()));

	TimeManager->SleepDelegate.AddDynamic(this, &ABed::MovePlayerToBed);
	Root->OnComponentBeginOverlap.AddDynamic(this, &ABed::OnBedBeginOverlap);
	Root->OnComponentEndOverlap.AddDynamic(this, &ABed::OnBedEndOverlap);
}

// Called every frame
void ABed::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABed::OnBedBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                             AActor* OtherActor, UPrimitiveComponent* OtherComp,
                             int32 OtherBodyIndex,
                             bool bFromSweep, const FHitResult& SweepResult)
{
	auto* player = Cast<AMainPlayerCharacter>(OtherActor);
	if (player)
	{
		UE_LOG(LogTemp, Warning, TEXT("Bed : Player begin overlap"));
		player->SetbLieOnBed(true);
		MainPlayer = player;
	}
}

void ABed::OnBedEndOverlap(UPrimitiveComponent* OverlappedComponent,
                           AActor* OtherActor, UPrimitiveComponent* OtherComp,
                           int32 OtherBodyIndex)
{
	auto* player = Cast<AMainPlayerCharacter>(OtherActor);
	if (player)
	{
		UE_LOG(LogTemp, Warning, TEXT("Bed : Player end overlap"));
		player->SetbLieOnBed(false);
		MainPlayer = nullptr;
	}
}

void ABed::MovePlayerToBed()
{
	MainPlayer->SetActorLocation(GetActorLocation());
}
