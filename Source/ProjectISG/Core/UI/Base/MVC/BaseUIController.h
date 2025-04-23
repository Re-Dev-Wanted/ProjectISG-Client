#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BaseUIController.generated.h"

UCLASS()
class PROJECTISG_API UBaseUIController : public UObject
{
	GENERATED_BODY()

public:
	virtual void InitializeController();
};
