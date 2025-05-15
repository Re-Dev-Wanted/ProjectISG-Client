// LootContainerData.cpp
// Complete implementations for functions in LootContainerData.h

#include "LootContainerData.h"
#include "ProjectISG/Systems/Inventory/ItemData.h" // Add this include

bool FLootContainerData::Contains(FGuid Guid)
{
    for (const FLootContainerEntry& Container : Containers)
    {
        if (Container.Guid == Guid)
        {
            return true;
        }
    }
    return false;
}

void FLootContainerData::AddContainer(FGuid NewGuid, int32 Capacity)
{
    // Check if container already exists
    if (Contains(NewGuid))
    {
        return;
    }

    FLootContainerEntry NewContainer;
    NewContainer.Guid = NewGuid;
    
    // Initialize with empty slots based on capacity
    NewContainer.Items.SetNum(Capacity);
    
    // Mark array for replication
    Containers.Add(NewContainer);
    MarkItemDirty(NewContainer);
}

void FLootContainerData::RemoveContainer(FGuid Guid)
{
    for (int32 i = 0; i < Containers.Num(); i++)
    {
        if (Containers[i].Guid == Guid)
        {
            Containers.RemoveAt(i);
            MarkArrayDirty();
            break;
        }
    }
}

void FLootContainerData::UpdateContainer(FGuid Guid, TArray<FItemMetaInfo> Items)
{
    for (int32 i = 0; i < Containers.Num(); i++)
    {
        if (Containers[i].Guid == Guid)
        {
            // Convert to FItemMetaInfo_Net
            Containers[i].Items.Empty(Items.Num());
            for (const FItemMetaInfo& Item : Items)
            {
                Containers[i].Items.Add(FItemMetaInfo_Net(Item));
            }
            MarkItemDirty(Containers[i]);
            break;
        }
    }
}

void FLootContainerData::UpdateContainer(FGuid Guid, const FItemMetaInfo& Item, uint16 Index)
{
    for (int32 i = 0; i < Containers.Num(); i++)
    {
        if (Containers[i].Guid == Guid)
        {
            if (Index < Containers[i].Items.Num())
            {
                Containers[i].Items[Index] = FItemMetaInfo_Net(Item);
                MarkItemDirty(Containers[i]);
            }
            break;
        }
    }
}

void FLootContainerData::SwapItemInContainer(FGuid Guid, uint16 Prev, uint16 Next)
{
    for (int32 i = 0; i < Containers.Num(); i++)
    {
        if (Containers[i].Guid == Guid)
        {
            if (Prev < Containers[i].Items.Num() && Next < Containers[i].Items.Num())
            {
                Swap(Containers[i].Items[Prev], Containers[i].Items[Next]);
                MarkItemDirty(Containers[i]);
            }
            break;
        }
    }
}

TArray<FItemMetaInfo> FLootContainerData::GetItems(FGuid Guid)
{
    TArray<FItemMetaInfo> Result;
    
    for (const FLootContainerEntry& Container : Containers)
    {
        if (Container.Guid == Guid)
        {
            Result.Reserve(Container.Items.Num());
            for (const FItemMetaInfo_Net& NetItem : Container.Items)
            {
                FItemMetaInfo Item;
                NetItem.To(Item);
                Result.Add(Item);
            }
            break;
        }
    }
    
    return Result;
}
