#include "Hammer.h"

AHammer::AHammer()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComp->SetRelativeLocation(FVector(-60.f, 0, 0));
}



