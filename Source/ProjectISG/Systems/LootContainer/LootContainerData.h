#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "UObject/Object.h"
#include "LootContainerData.generated.h"

struct FItemMetaInfo;

USTRUCT(BlueprintType)
struct FLootContainerEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	UPROPERTY()
	FGuid Guid = FGuid();

	UPROPERTY()
	TArray<FItemMetaInfo> Items;

	bool IsValid() const
	{
		return Guid.IsValid();
	}
};

USTRUCT(BlueprintType)
struct FLootContainerData : public FFastArraySerializer
{
	GENERATED_BODY()

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams);

public:
	FGuid AddContainer(int32 Capacity);

	void RemoveContainer(FGuid Guid);

	void UpdateContainer(FGuid Guid, TArray<FItemMetaInfo> Items);

	void UpdateContainer(FGuid Guid, const FItemMetaInfo& Item, uint16 Index);

	TArray<FItemMetaInfo> GetItems(FGuid Guid);
	
protected:
	UPROPERTY()
	TArray<FLootContainerEntry> Containers;
};
