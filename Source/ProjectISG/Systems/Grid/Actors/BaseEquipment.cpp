#include "BaseEquipment.h"

ABaseEquipment::ABaseEquipment()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(RootComponent);

	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bReplicates = true;
}

void ABaseEquipment::BeginPlay()
{
	Super::BeginPlay();

	SetReplicateMovement(true);

	MeshComp->SetRenderCustomDepth(true);
}

