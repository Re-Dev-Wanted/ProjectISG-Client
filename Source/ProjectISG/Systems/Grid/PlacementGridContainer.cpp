#include "PlacementGridContainer.h"

template<>
struct TStructOpsTypeTraits<FPlacementGridContainer> : TStructOpsTypeTraitsBase2<FPlacementGridContainer>
{
	enum
	{
		WithNetDeltaSerializer = true
	};
};

bool FPlacementGridContainer::NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
{
	return FastArrayDeltaSerialize<FPlacementGridEntry, FPlacementGridContainer>(Items, DeltaParms, *this);
}

void FPlacementGridContainer::Add(const FIntVector& GridCoord, APlacement* Placement, uint16 ItemId, TFunction<void(void)>&& OnUpdateCallback)
{
	APlacement* TypedPlacement = Cast<APlacement>(Placement);

	if (!TypedPlacement)
	{
		return;
	}

	FPlacementGridEntry Entry;
	Entry.GridCoord = GridCoord;
	Entry.Placement = TypedPlacement;
	Entry.ItemId = ItemId;
	Items.Add(Entry);

	MarkItemDirty(Entry);

	PlacedMap.Add(GridCoord, Placement);

	if (OnUpdateCallback)
	{
		OnUpdateCallback();
	}
}

uint16 FPlacementGridContainer::Remove(APlacement* Placement, TFunction<void(void)>&& OnUpdateCallback)
{
	uint16 ItemId = 0;

	// 모든 관련 좌표 제거
	TArray<int32> IndicesToRemove;

	for (int32 i = 0; i < GetItems().Num(); ++i)
	{
		const FPlacementGridEntry& Entry = GetItems()[i];

		if (Entry.Placement == Placement)
		{
			if (ItemId == 0)
			{
				ItemId = Entry.ItemId;
			}
			
			IndicesToRemove.Add(i);
		}
	}

	if (IndicesToRemove.Num() == 0)
	{
		return 0;
	}

	// 실제 배열에서 제거
	for (int32 Index : IndicesToRemove)
	{
		GetItems().RemoveAt(Index);
	}

	// 캐시 Map도 정리
	for (auto It = GetPlacedMap().CreateIterator(); It; ++It)
	{
		if (It.Value() == Placement)
		{
			It.RemoveCurrent();
		}
	}
	
	MarkArrayDirty();

	if (OnUpdateCallback)
	{
		OnUpdateCallback();
	}

	return ItemId;
}
