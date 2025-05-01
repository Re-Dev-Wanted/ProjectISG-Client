#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "ProjectISG/Systems/Inventory/ItemData.h"
#include "Actors/Placement.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "PlacementGridContainer.generated.h"

USTRUCT()
struct FPlacementGridEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	UPROPERTY()
	FIntVector GridCoord;

	UPROPERTY()
	TWeakObjectPtr<APlacement> Placement;

	UPROPERTY()
	uint16 ItemId;

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

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms);

	void Add(const FIntVector& GridCoord, class APlacement* Placement, uint16 ItemId);

	uint16 Remove(class APlacement* Placement);

	TMap<FIntVector, TWeakObjectPtr<APlacement>> GetPlacedMap() const
	{
		return PlacedMap;
	}

protected:
	UPROPERTY()
	TArray<FPlacementGridEntry> Items;

	//빠른 접근용 Cache Map
	UPROPERTY(NotReplicated)
	TMap<FIntVector, TWeakObjectPtr<APlacement>> PlacedMap;

	TMap<TWeakObjectPtr<APlacement>, TArray<FIntVector>> ReverseMap;
};
