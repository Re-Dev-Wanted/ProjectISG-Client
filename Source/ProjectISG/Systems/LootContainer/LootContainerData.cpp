#include "LootContainerData.h"

#include "ProjectISG/Systems/Inventory/ItemData.h"

template<>
struct TStructOpsTypeTraits<FLootContainerData> : TStructOpsTypeTraitsBase2<FLootContainerData>
{
	enum
	{
		WithNetDeltaSerializer = true
	};
};

bool FLootContainerData::NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
{
	return FastArrayDeltaSerialize<FLootContainerEntry, FLootContainerData>
	(Containers, DeltaParams, *this);
}

FGuid FLootContainerData::AddContainer(int32 Capacity)
{
	FGuid Guid = FGuid::NewGuid();
	FLootContainerEntry& Entry = Containers.AddDefaulted_GetRef();
	Entry.Guid = Guid;
	Entry.Items.Init(FItemMetaInfo(), Capacity);

	MarkItemDirty(Entry);

	return Guid;
}

void FLootContainerData::RemoveContainer(FGuid Guid)
{
	for (auto It = Containers.CreateIterator(); It; ++It)
	{
		if (It->Guid == Guid)
		{
			int32 Index = It.GetIndex();
			Containers.RemoveAt(Index);
			MarkArrayDirty();
			break;
		}
	}
}

void FLootContainerData::UpdateContainer(FGuid Guid,
	TArray<FItemMetaInfo> Items)
{
	for (FLootContainerEntry& Entry : Containers)
	{
		if (Entry.Guid == Guid)
		{
			Entry.Items = Items;
			MarkArrayDirty();
			break;
		}
	}
}

void FLootContainerData::UpdateContainer(FGuid Guid, const FItemMetaInfo& Item, uint16 Index)
{
	for (FLootContainerEntry& Entry : Containers)
	{
		if (Entry.Guid == Guid)
		{
			Entry.Items[Index] = Item;
			MarkArrayDirty();
			break;
		}
	}
}

void FLootContainerData::SwapItemInContainer(FGuid Guid, uint16 Prev, uint16 Next)
{
	for (FLootContainerEntry& Entry : Containers)
	{
		if (Entry.Guid == Guid)
		{
			FItemMetaInfo Temp = Entry.Items[Prev];
			
			Entry.Items[Prev] = Entry.Items[Next];
			Entry.Items[Next] = Temp;
			
			MarkArrayDirty();
			break;
		}
	}
}

TArray<FItemMetaInfo> FLootContainerData::GetItems(FGuid Guid)
{
	for (FLootContainerEntry& Entry : Containers)
	{
		if (Entry.Guid == Guid)
		{
			return Entry.Items;
		}
	}

	return TArray<FItemMetaInfo>();
}
