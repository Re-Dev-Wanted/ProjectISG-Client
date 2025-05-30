#include "FishActor.h"

AFishActor::AFishActor()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(MeshComp);
}

void AFishActor::SetMesh(TObjectPtr<UStaticMesh> Mesh)
{
	MeshComp->SetStaticMesh(Mesh);
}

