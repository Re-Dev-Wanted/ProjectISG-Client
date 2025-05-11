#include "BaseEquipment.h"

ABaseEquipment::ABaseEquipment()
{
	PrimaryActorTick.bCanEverTick = false;

	AnchorComp = CreateDefaultSubobject<USceneComponent>(TEXT("Anchor"));
	AnchorComp->SetupAttachment(RootComponent);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(AnchorComp);

	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bReplicates = true;
}

void ABaseEquipment::BeginPlay()
{
	Super::BeginPlay();

	SetReplicateMovement(true);
}

