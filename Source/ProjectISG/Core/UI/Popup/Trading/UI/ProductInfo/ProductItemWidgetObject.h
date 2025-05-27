#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ProductItemWidgetObject.generated.h"

UCLASS()
class PROJECTISG_API UProductItemWidgetObject : public UObject
{
	GENERATED_BODY()

public:
	uint16 ProductId;
};
