#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "PlacementGridContainer.generated.h"

USTRUCT()
struct FPlacementGridEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	UPROPERTY()
	FIntVector GridCoord;

	UPROPERTY()
	TWeakObjectPtr<class APlacement> Placement;

	bool IsValid() const
	{
		return Placement.IsValid();
	}
};

USTRUCT()
struct FPlacementGridContainer : public FFastArraySerializer
{
	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms);

	GENERATED_BODY()

	UPROPERTY()
	TArray<FPlacementGridEntry> Items;

	UPROPERTY(NotReplicated)
	class AGridManager* Owner = nullptr;

	//빠른 접근용 Cache Map
	UPROPERTY(NotReplicated)
	TMap<FIntVector, TWeakObjectPtr<class APlacement>> PlacedMap;

	TMap<TWeakObjectPtr<class APlacement>, TArray<FIntVector>> ReverseMap;

	void Add(const FIntVector& GridCoord, class APlacement* Placement);

	void Remove(class APlacement* Placement);
	
};
