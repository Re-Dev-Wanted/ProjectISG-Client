#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Utils/MacroUtil.h"

#include "ItemData.generated.h"

UENUM()
enum class EItemType : uint32
{
	Undefined,
	Interactive,
	Equipment,
	Consume,
	Ingredient,
	Build,
	Max,
};

// 아이템 인스턴스 하나하나의 고유한 값을 저장할 때 사용된다.
UENUM()
enum class EMetaDataKey : uint32
{
	None,
	Durability,
};

// 아이템 데이터 전체에서 고유한 값을 저장할 때 사용된다.
UENUM()
enum class EConstDataKey : uint32
{
	None,
	MaxDurability,
	ItemUseType,
	SocketName,
	GameplayTag,
	GeneratedItemId, // 해당 아이템이 다른 아이템을 만들 경우
};

// 아이템 정보를 담아 추후 아이템을 구성할 때 사용할 요소
USTRUCT(BlueprintType)
struct PROJECTISG_API FItemInfoData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	GETTER(FString, DisplayName)
	GETTER(EItemType, ItemType)
	GETTER(TSoftObjectPtr<UTexture2D>, Thumbnail)
	GETTER(TSubclassOf<AActor>, ShowItemActor)
	GETTER(TSubclassOf<AActor>, PlaceItemActor)
	GETTER(uint32, MaxItemCount);

	FORCEINLINE TMap<EMetaDataKey, FString> GetMetaData() const
	{
		return MetaData;
	}

	FORCEINLINE TMap<EConstDataKey, FString> GetConstData() const
	{
		return ConstData;
	}

private:
	UPROPERTY(EditDefaultsOnly, Category = "Data",
		meta=(AllowPrivateAccess = true))
	;
	FString DisplayName;

	UPROPERTY(EditDefaultsOnly, Category = "Data",
		meta=(AllowPrivateAccess = true))
	;
	EItemType ItemType = EItemType::Undefined;

	UPROPERTY(EditDefaultsOnly, Category = "Data",
		meta=(AllowPrivateAccess = true))
	;
	TSoftObjectPtr<UTexture2D> Thumbnail;

	// 드롭될 때 나올 아이템 액터 정보
	// TODO: 액터는 추후 아이템 액터 공통화로 처리한다.
	UPROPERTY(EditDefaultsOnly, Category = "Data",
		meta=(AllowPrivateAccess = true))
	;
	TSubclassOf<AActor> ShowItemActor;

	UPROPERTY(EditDefaultsOnly, Category = "Data",
		meta=(AllowPrivateAccess = true))
	;
	TSubclassOf<AActor> DroppedItemActor;

	UPROPERTY(EditDefaultsOnly, Category = "Data",
		meta=(AllowPrivateAccess = true))
	;
	TSubclassOf<AActor> PlaceItemActor;

	UPROPERTY(EditDefaultsOnly, Category = "Data",
		meta = (AllowPrivateAccess = true,
			ClampMin = 0, ClampMax = 1000, UIMin = 0, UIMax = 1000))
	;
	uint32 MaxItemCount = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Data",
		meta=(AllowPrivateAccess = true))
	;
	TMap<EMetaDataKey, FString> MetaData;

	UPROPERTY(EditDefaultsOnly, Category = "Data",
		meta=(AllowPrivateAccess = true))
	;
	TMap<EConstDataKey, FString> ConstData;
};

// 실제 플레이어가 저장할 정보 값
USTRUCT(BlueprintType)
struct PROJECTISG_API FItemMetaInfo
{
	GENERATED_USTRUCT_BODY()

	GETTER_SETTER(uint16, Id)
	GETTER(uint32, CurrentCount)

	FORCEINLINE TMap<EMetaDataKey, FString> GetMetaData() const
	{
		return MetaData;
	}

	FORCEINLINE void SetMetaData(
		const TMap<EMetaDataKey, FString>& NewMetaData)
	{
		MetaData = NewMetaData;
	}

	FORCEINLINE void SetCurrentCount(const int NewValue)
	{
		CurrentCount = NewValue;
	}

	bool operator ==(const FItemMetaInfo& CompareItem) const
	{
		return CompareItem.GetId() == GetId();
	}

	bool IsValid() const
	{
		return Id > 0;
	}

private:
	// 0인 경우 아이템이 안들어가게 처리해야함.
	UPROPERTY(EditDefaultsOnly, Category = "Data", meta =
		(ClampMin = 0, ClampMax = 1000, UIMin = 0, UIMax = 1000))
	uint16 Id = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Data", meta =
		(ClampMin = 0, ClampMax = 1000, UIMin = 0, UIMax = 1000))
	uint32 CurrentCount = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Data")
	TMap<EMetaDataKey, FString> MetaData;
};


USTRUCT(BlueprintType)
struct FItemMetaInfo_Net
{
	GENERATED_BODY()

	UPROPERTY()
	int32 Id = 0;

	UPROPERTY()
	int Count = 0;

	UPROPERTY()
	TArray<EMetaDataKey> Keys;

	UPROPERTY()
	TArray<FString> Values;

	FItemMetaInfo_Net()
	{
	}

	FItemMetaInfo_Net(const FItemMetaInfo& Original)
	{
		Id = Original.GetId();
		Count = Original.GetCurrentCount();

		for (const auto& Pair : Original.GetMetaData())
		{
			Keys.Add(Pair.Key);
			Values.Add(Pair.Value);
		}
	}

	void To(FItemMetaInfo& OutData) const
	{
		OutData.SetId(Id);
		OutData.SetCurrentCount(Count);
		TMap<EMetaDataKey, FString> MetaData;

		for (int32 i = 0; i < Keys.Num(); ++i)
		{
			MetaData.Add(Keys[i], Values[i]);
		}

		OutData.SetMetaData(MetaData);
	}
};
