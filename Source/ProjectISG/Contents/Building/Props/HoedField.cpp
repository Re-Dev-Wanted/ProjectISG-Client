#include "HoedField.h"

void AHoedField::BeginPlay()
{
	Super::BeginPlay();

	MeshComp->SetRenderCustomDepth(false);
}

