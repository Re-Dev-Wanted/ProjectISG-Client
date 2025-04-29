#include "BaseEquipment.h"

ABaseEquipment::ABaseEquipment()
{
	PrimaryActorTick.bCanEverTick = false;

	AnchorComp = CreateDefaultSubobject<USceneComponent>(TEXT("Anchor"));
	AnchorComp->SetupAttachment(RootComponent);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(AnchorComp);

	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

