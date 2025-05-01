#pragma once
#include "ProjectISG/Utils/MacroUtil.h"
#include "ProductStruct.generated.h"

USTRUCT(BlueprintType)
struct PROJECTISG_API FProductStruct : public FTableRowBase
{
	GENERATED_BODY()

	GETTER(uint32, ProductId);
	GETTER(uint32, ProductPrice);

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	uint32 ProductId = 0;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	uint32 ProductPrice = 0;
};
