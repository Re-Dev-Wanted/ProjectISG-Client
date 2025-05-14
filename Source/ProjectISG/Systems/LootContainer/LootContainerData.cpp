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

FGuid FLootContainerData::AddContainer()
{
	FGuid Guid = FGuid::NewGuid();
	FLootContainerEntry& Entry = Containers.AddDefaulted_GetRef();
	Entry.Guid = Guid;

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
