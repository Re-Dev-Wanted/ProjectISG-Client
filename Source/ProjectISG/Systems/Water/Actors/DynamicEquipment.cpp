#include "DynamicEquipment.h"

ADynamicEquipment::ADynamicEquipment()
{
	PrimaryActorTick.bCanEverTick = false;

	AnchorComp = CreateDefaultSubobject<USceneComponent>(TEXT("Anchor"));
	AnchorComp->SetupAttachment(RootComponent);

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(AnchorComp);

	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

