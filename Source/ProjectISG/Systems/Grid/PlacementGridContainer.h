#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "ProjectISG/Systems/Inventory/ItemData.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "PlacementGridContainer.generated.h"

USTRUCT()
struct FPlacementGridEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	UPROPERTY()
	FIntVector GridCoord;

	UPROPERTY()
	TWeakObjectPtr<class APlacement> Placement;

	UPROPERTY()
	FItemMetaInfo ItemMetaInfo;

	bool IsValid() const
	{
		return Placement.IsValid();
	}
};

USTRUCT()
struct FPlacementGridContainer : public FFastArraySerializer
{
	GENERATED_BODY()

	GETTER(TArray<FPlacementGridEntry>, Items)
	SETTER(class AGridManager*, Owner)

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms);

	void Add(const FIntVector& GridCoord, class APlacement* Placement, FItemMetaInfo ItemMetaInfo);

	FItemMetaInfo Remove(class APlacement* Placement);

	TMap<FIntVector, TWeakObjectPtr<class APlacement>> GetPlacedMap() const
	{
		return PlacedMap;
	}

protected:
	UPROPERTY(NotReplicated)
	class AGridManager* Owner = nullptr;
	
	UPROPERTY()
	TArray<FPlacementGridEntry> Items;

	//빠른 접근용 Cache Map
	UPROPERTY(NotReplicated)
	TMap<FIntVector, TWeakObjectPtr<class APlacement>> PlacedMap;

	TMap<TWeakObjectPtr<class APlacement>, TArray<FIntVector>> ReverseMap;
};
