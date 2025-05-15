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
	virtual FItemMetaInfo GetItemMetaInfo(FGuid Guid, const uint16 Index) = 0;
	
	virtual bool ChangeItem(FGuid Guid, const FItemMetaInfo& ItemInfo, const uint16 Index) = 0;

	virtual void SwapItem(FGuid Guid, const uint16 Prev, const uint16 Next) = 0;
};
