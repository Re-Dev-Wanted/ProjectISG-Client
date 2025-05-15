#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "UObject/Object.h"
#include "LootContainerData.generated.h"

struct FItemMetaInfo_Net;
struct FItemMetaInfo;

USTRUCT(BlueprintType)
struct FLootContainerEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	UPROPERTY()
	FGuid Guid = FGuid();

	UPROPERTY()
	TArray<FItemMetaInfo_Net> Items;

	bool IsValid() const
	{
		return Guid.IsValid();
	}

	void PostReplicatedAdd(const struct FFastArraySerializer& InArraySerializer)
	{
		UE_LOG(LogTemp, Warning, TEXT("???????? [%d]"), Items.Num());
	}

	void PostReplicatedChange(const struct FFastArraySerializer& InArraySerializer)
	{
		UE_LOG(LogTemp, Warning, TEXT("PostReplicatedChange [%d]"), Items.Num());
	}
};

USTRUCT(BlueprintType)
struct FLootContainerData : public FFastArraySerializer
{
	GENERATED_BODY()

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
	{
		return FastArrayDeltaSerialize<FLootContainerEntry, FLootContainerData>(Containers, DeltaParams, *this);
	}
	
	bool Contains(FGuid Guid);
	
	void AddContainer(FGuid NewGuid, int32 Capacity);

	void RemoveContainer(FGuid Guid);

	void UpdateContainer(FGuid Guid, TArray<FItemMetaInfo> Items);

	void UpdateContainer(FGuid Guid, const FItemMetaInfo& Item, uint16 Index);

	void SwapItemInContainer(FGuid Guid, uint16 Prev, uint16 Next);
	
	TArray<FItemMetaInfo> GetItems(FGuid Guid);

	TArray<FLootContainerEntry>& GetContainersRef()
	{
		return Containers;
	}

	void PostReplicatedAdd(const TArrayView<int32>& AddedIndices, int32 FinalSize)
	{
		UE_LOG(LogTemp, Warning, TEXT("PostReplicatedAdd [%d]"), FinalSize);
	}


protected:
	UPROPERTY()
	TArray<FLootContainerEntry> Containers;
};

template<>
struct TStructOpsTypeTraits<FLootContainerData> : TStructOpsTypeTraitsBase2<FLootContainerData>
{
	enum
	{
		WithNetDeltaSerializer = true
	};
};
