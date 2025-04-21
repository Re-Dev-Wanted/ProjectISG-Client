// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCrop.h"

#include "Components/BoxComponent.h"


// Sets default values
ABaseCrop::ABaseCrop()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	Super::SetReplicateMovement(true);


	Root = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
}

// Called when the game starts or when spawned
void ABaseCrop::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABaseCrop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
