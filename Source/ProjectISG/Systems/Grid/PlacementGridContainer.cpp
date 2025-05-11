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

	FPlacementGridEntry& Entry = Items.AddDefaulted_GetRef();
	
	Entry.GridCoord = GridCoord;
	Entry.Placement = TypedPlacement;
	Entry.ItemId = ItemId;

	MarkItemDirty(Entry);
	MarkArrayDirty();

	if (OnUpdateCallback)
	{
		OnUpdateCallback();
	}
}

uint16 FPlacementGridContainer::Remove(const FIntVector& GridCoord, uint16 ItemId, TFunction<void(void)>&& OnUpdateCallback)
{
	// 모든 관련 좌표 제거
	TArray<int32> IndicesToRemove;

	for (int32 i = 0; i < GetItems().Num(); ++i)
	{
		const FPlacementGridEntry& Entry = Items[i];

		if (Entry.GridCoord == GridCoord && Entry.ItemId == ItemId)
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
		Items.RemoveAt(Index);
	}
	
	MarkArrayDirty();

	if (OnUpdateCallback)
	{
		OnUpdateCallback();
	}

	return ItemId;
}
