#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Systems/Grid/Actors/Placement.h"
#include "LightFurniture.generated.h"

class UPointLightComponent;
class UNiagaraComponent;

UCLASS()
class PROJECTISG_API ALightFurniture : public APlacement
{
	GENERATED_BODY()

public:
	ALightFurniture();

	virtual void SetOption(bool bIsGhost, bool bIsBlock = false) override;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> EffectComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPointLightComponent> LightComp;
};
