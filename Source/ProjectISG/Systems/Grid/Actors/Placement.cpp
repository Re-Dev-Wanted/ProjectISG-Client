#include "Placement.h"

#include "Components/BoxComponent.h"
#include "ProjectISG/Systems/Grid/Manager/GridManager.h"


APlacement::APlacement()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	CollisionComp->SetupAttachment(RootComponent);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(CollisionComp);
}

void APlacement::BeginPlay()
{
	Super::BeginPlay();

	// Setup();
}

void APlacement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlacement::Setup()
{
	// 오브젝트 크기가 제각각 다를 것이다.
	// 적당히 가운데 정렬하고 tileSize에 맞추기

	FVector BoxExtent = StaticMesh->GetBounds().BoxExtent;

	CollisionComp->SetBoxExtent(BoxExtent);
	CollisionComp->SetRelativeLocation(BoxExtent);

	MeshComp->SetStaticMesh(StaticMesh);
}
