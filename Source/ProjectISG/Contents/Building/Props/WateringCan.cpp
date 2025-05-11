#include "WateringCan.h"

AWateringCan::AWateringCan()
{
	MeshComp->SetRelativeLocation(FVector(-18.0f, -20.f, 0));
	MeshComp-> SetRelativeRotation(FRotator(-90.f, 90.f, 0.f));
	MeshComp->SetRelativeScale3D(FVector::OneVector * 1.5f);
}

