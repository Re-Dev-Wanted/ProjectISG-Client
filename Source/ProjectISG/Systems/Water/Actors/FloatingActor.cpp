#include "FloatingActor.h"
#include "Components/BoxComponent.h"
#include "ProjectISG/Systems/Water/Components/BuoyancyComponent.h"

AFloatingActor::AFloatingActor()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Root->SetSimulatePhysics(true);
	Root->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(Root);

	BuoyancyComponent = CreateDefaultSubobject<UBuoyancyComponent>(TEXT("BuoyancyComponent"));
}

bool AFloatingActor::CanApplyBuoyancy() const
{
	return true;
}

float AFloatingActor::GetBuoyancyScale() const
{
	return 1.f;
}

float AFloatingActor::GetActorBottomZ() const
{
	FVector Origin;
	FVector Extent;
	
	GetActorBounds(true, Origin, Extent);
	
	return Origin.Z - Extent.Z;
}

