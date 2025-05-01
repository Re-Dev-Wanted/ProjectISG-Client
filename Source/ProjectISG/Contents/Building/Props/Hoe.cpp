#include "Hoe.h"

AHoe::AHoe()
{
	MeshComp->SetRelativeLocation(FVector(60.f, 0, 0));
	MeshComp->SetRelativeRotation(FRotator(0, -180.f, 180.f));
	MeshComp->SetRelativeScale3D(FVector::OneVector * 0.5f);
}


