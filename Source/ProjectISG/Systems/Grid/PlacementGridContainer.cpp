#include "PlacementGridContainer.h"
#include "Actors/Placement.h"
#include "Manager/GridManager.h"

template<>
struct TStructOpsTypeTraits<FPlacementGridContainer> : public TStructOpsTypeTraitsBase2<FPlacementGridContainer>
{
	enum
	{
		WithNetDeltaSerializer = true
	};
};

bool FPlacementGridContainer::NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
{
	return FFastArraySerializer::FastArrayDeltaSerialize<FPlacementGridEntry, FPlacementGridContainer>(Items, DeltaParms, *this);
}

void FPlacementGridContainer::Add(const FIntVector& GridCoord, APlacement* Placement)
{
	APlacement* TypedPlacement = Cast<APlacement>(Placement);

	if (!TypedPlacement)
	{
		return;
	}

	FPlacementGridEntry Entry;
	Entry.GridCoord = GridCoord;
	Entry.Placement = TypedPlacement;
	Items.Add(Entry);

	MarkItemDirty(Entry);

	if (Owner)
	{
		// Clone Trigger
		Owner->ForceNetUpdate();
	}

	PlacedMap.Add(GridCoord, Placement);
}

void FPlacementGridContainer::Remove(APlacement* Placement)
{
	Items.RemoveAll
	([&]
		(const FPlacementGridEntry& Entry)
		{
			return Entry.Placement == Placement;
		}
	);

	for (auto It = PlacedMap.CreateIterator(); It; ++It)
	{
		if (It.Value() == Placement)
		{
			It.RemoveCurrent();
		}
	}
	
	MarkArrayDirty();
}
