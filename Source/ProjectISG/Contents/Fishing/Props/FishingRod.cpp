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
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SocketComp = CreateDefaultSubobject<USceneComponent>(TEXT("SocketComp"));
	SocketComp->SetupAttachment(MeshComp, TEXT("ThreadSocket"));

	PocketSocketComp = CreateDefaultSubobject<USceneComponent>(TEXT("PocketSocketComp"));
	PocketSocketComp->SetupAttachment(MeshComp, TEXT("PocketSocket"));

	CastingStartPoint = CreateDefaultSubobject<USceneComponent>(TEXT("CastingStartPoint"));
	CastingStartPoint->SetupAttachment(MeshComp);

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
		Bobber->OnEnterWater.BindDynamic(this, &AFishingRod::OnStartFishing);
		Bobber->SetCollisionAndPhysicsEnabled(false);
		Bobber->AttachToComponent(PocketSocketComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

		Thread->SetAttachEndTo(Bobber, TEXT("LineAttachPoint"));
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

void AFishingRod::OnStartFishing()
{
	float WaitTime = FMath::RandRange(3.f, 7.f);
	
	GetWorld()->
	GetTimerManager()
	.SetTimer
	(
		FishingTimerHandle,
		this,
		&AFishingRod::OnEventBite,
		WaitTime,
		false
	);
}

void AFishingRod::OnEventBite()
{
	if (!Bobber)
	{
		return;
	}

	Bobber->OnBite();
}

void AFishingRod::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AFishingRod::GetCanTouch() const
{
	return true;
}

FString AFishingRod::GetDisplayText() const
{
	return TEXT("올리기");
}

void AFishingRod::OnTouch(AActor* Causer)
{
	Super::OnTouch(Causer);
}

void AFishingRod::OnTouchResponse()
{
	Super::OnTouchResponse();
}

void AFishingRod::StartCasting(FVector Destination)
{
	if (!Bobber)
	{
		return;
	}

	Bobber->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Bobber->SetActorLocation(CastingStartPoint->GetComponentLocation());
	
	Bobber->SuggestProjectileVelocity(CastingStartPoint->GetComponentLocation(),Destination);
}
