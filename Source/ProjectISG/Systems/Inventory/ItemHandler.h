#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "UObject/Interface.h"
#include "ItemHandler.generated.h"

UINTERFACE(BlueprintType)
class UItemHandler : public UInterface
{
	GENERATED_BODY()
};


class PROJECTISG_API IItemHandler
{
	GENERATED_BODY()
public:
	virtual bool ChangeItem(FGuid Guid, const FItemMetaInfo& ItemInfo, const uint16 Index) { return false; }
};
