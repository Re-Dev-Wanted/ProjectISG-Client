// Fill out your copyright notice in the Description page of Project Settings.

#include "FishingRod.h"

#include "Bobber.h"
#include "CableComponent.h"

// Sets default values
AFishingRod::AFishingRod()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp->SetRelativeLocation(FVector(0.f, 0.f, 5.f));
	MeshComp->SetRelativeScale3D(FVector::OneVector * 0.3f);
	MeshComp->SetRelativeRotation(FRotator(0.f, -90.f, 180.f));

	SocketComp = CreateDefaultSubobject<USceneComponent>(TEXT("SocketComp"));
	SocketComp->SetupAttachment(MeshComp, TEXT("ThreadSocket"));

	PocketSocketComp = CreateDefaultSubobject<USceneComponent>(TEXT("PocketSocketComp"));
	PocketSocketComp->SetupAttachment(MeshComp, TEXT("PocketSocket"));

	Thread = CreateDefaultSubobject<UCableComponent>(TEXT("ThreadComp"));
	Thread->SetupAttachment(SocketComp);
	Thread->bAttachEnd = true;
	Thread->SetAttachEndTo(this, TEXT("PocketSocketComp"));
	Thread->EndLocation = FVector::ZeroVector;
}


void AFishingRod::BeginPlay()
{
	Super::BeginPlay();

	if (!Bobber)
	{
		Bobber = GetWorld()->SpawnActor<ABobber>(BobberFactory);
		Bobber->SetCollisionAndPhysicsEnabled(false);
		Bobber->AttachToComponent(PocketSocketComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
}

void AFishingRod::Destroyed()
{
	Super::Destroyed();

	if (Bobber)
	{
		Bobber->Destroy();
	}
}

void AFishingRod::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

