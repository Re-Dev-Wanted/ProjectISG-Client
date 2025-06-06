#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "UObject/Object.h"
#include "ProductItemWidgetObject.generated.h"

UCLASS()
class PROJECTISG_API UProductItemWidgetObject : public UObject
{
	GENERATED_BODY()

public:
	GETTER_SETTER(uint16, ProductId)
	GETTER_SETTER(float, PriceRatio)
	GETTER_SETTER(uint32, ProductCount)

private:
	uint16 ProductId;
	float PriceRatio;
	uint32 ProductCount;
};
