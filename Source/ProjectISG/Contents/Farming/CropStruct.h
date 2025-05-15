#pragma once
#include "ProjectISG/Utils/MacroUtil.h"
#include "CropStruct.generated.h"

USTRUCT(BlueprintType)
struct PROJECTISG_API FCropStruct : public FTableRowBase
{
	GENERATED_BODY()

public:
	GETTER(uint16, CropId)

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	uint16 CropId = 0;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	uint16 ItemId = 0;

};