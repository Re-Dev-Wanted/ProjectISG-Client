// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseConsume.h"

#include "Components/BoxComponent.h"


// Sets default values
ABaseConsume::ABaseConsume()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	Root = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Root->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
	Mesh->SetRenderCustomDepth(true);
}

// Called when the game starts or when spawned
void ABaseConsume::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseConsume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

