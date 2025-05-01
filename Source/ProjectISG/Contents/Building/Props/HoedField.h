#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Systems/Grid/Actors/Placement.h"
#include "HoedField.generated.h"

UCLASS()
class PROJECTISG_API AHoedField : public APlacement
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
};
