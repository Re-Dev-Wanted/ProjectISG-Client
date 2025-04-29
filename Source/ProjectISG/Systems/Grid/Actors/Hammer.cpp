#include "Hammer.h"

#include "Kismet/KismetSystemLibrary.h"

AHammer::AHammer()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComp->SetRelativeLocation(FVector(-60.f, 0, 0));
}

void AHammer::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHammer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHammer::OnInteractive(AActor* Causer)
{
	IInteractionInterface::OnInteractive(Causer);
}

