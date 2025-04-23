#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BaseUIModel.generated.h"

UCLASS(Abstract, Blueprintable)
class PROJECTISG_API UBaseUIModel : public UObject
{
	GENERATED_BODY()

public:
	virtual void InitializeModel() PURE_VIRTUAL(UBaseUIModel::InitializeModel,);
};
