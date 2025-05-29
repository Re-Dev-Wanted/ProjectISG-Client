#include "LightFurniture.h"

#include "NiagaraComponent.h"
#include "Components/PointLightComponent.h"


ALightFurniture::ALightFurniture()
{
	PrimaryActorTick.bCanEverTick = true;

	EffectComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Effect"));
	EffectComp->SetupAttachment(MeshComp, TEXT("FireSocket"));

	LightComp = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light"));
	LightComp->SetupAttachment(EffectComp);
}

void ALightFurniture::SetOption(bool bIsGhost, bool bIsBlock)
{
	Super::SetOption(bIsGhost, bIsBlock);

	EffectComp->SetVisibility(!bIsGhost);
	LightComp->SetVisibility(!bIsGhost);
}

