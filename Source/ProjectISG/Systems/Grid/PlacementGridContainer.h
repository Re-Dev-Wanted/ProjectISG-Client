#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "Actors/Placement.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "PlacementGridContainer.generated.h"

USTRUCT()
struct FPlacementGridEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	UPROPERTY()
	FIntVector GridCoord = FIntVector::NoneValue;

	UPROPERTY()
	TObjectPtr<APlacement> Placement = nullptr;

	UPROPERTY()
	uint16 ItemId = 0;

	bool IsValid() const
	{
		return Placement != nullptr;
	}
};

USTRUCT()
struct FPlacementGridContainer : public FFastArraySerializer
{
	GENERATED_BODY()

	GETTER(TArray<FPlacementGridEntry>, Items)

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms);

	void Add(const FIntVector& GridCoord, APlacement* Placement, uint16 ItemId, TFunction<void(void)>&& OnUpdateCallback);

	uint16 Remove(const FIntVector& GridCoord, uint16 ItemId, TFunction<void(void)>&& OnUpdateCallback);

protected:
	UPROPERTY()
	TArray<FPlacementGridEntry> Items;
};
