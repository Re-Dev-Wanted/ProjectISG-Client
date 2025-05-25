#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectISG/Systems/Inventory/ItemData.h"
#include "ProjectISG/Systems/Inventory/ItemHandler.h"
#include "ProjectISG/Utils/MacroUtil.h"

#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdateNotified);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTISG_API UInventoryComponent : public UActorComponent, public IItemHandler
{
	GENERATED_BODY()

public:
	UInventoryComponent();

	GETTER(uint8, InventorySlotCount)
	GETTER(TArray<FItemMetaInfo>, InventoryList)

	uint32 GetCurrentCount(const uint16 Id);

	uint32 AddItem(const FItemMetaInfo& ItemInfo);

	bool HasItemInInventory(const uint32 Id, const uint32 Count);

	bool HasExactItemInInventory(const FItemMetaInfo& ItemMetaInfo);

	bool DropItem(const uint16 Index, const uint32 Count);

	// 특정 Id 기반의 아이템 삭제
	bool RemoveItem(const uint16 Id, const uint32 Count);

	// ItemMetaInfo 기반의 아이템 제거
	bool RemoveExactItem(const FItemMetaInfo& ItemInfo);

	uint32 AddItemToInventory(const uint16 Index,
	                          const FItemMetaInfo& ItemInfo);

	void SwapItemInInventory(const uint16 Prev, const uint16 Next);

	FORCEINLINE TMap<uint32, uint32> GetCurrentRemainItemValue() const
	{
		return CurrentRemainItemValue;
	}

	FORCEINLINE TMap<FItemMetaInfo, uint32> GetCurrentRemainItemMetaValue() const
	{
		return CurrentRemainItemMetaValue;
	}

	FOnInventoryUpdateNotified OnInventoryUpdateNotified;

	void InitializeItemData();
	
	virtual FItemMetaInfo GetItemMetaInfo(const uint16 Index) override;
	
	virtual bool ChangeItem(AActor* Causer, const FItemMetaInfo& ItemInfo, const uint16 Index) override;

	virtual void SwapItem(AActor* Causer, const uint16 Prev, const uint16 Next) override;

protected:
	virtual void BeginPlay() override;

	virtual void InitializeComponent() override;

private:
	void UpdateInventory_Internal();
	
	void UpdateCurrentRemainItemValue();
	
	// Id 기반의 간단한 아이템 갯수 데이터
	TMap<uint32, uint32> CurrentRemainItemValue;

	// 정확한 아이템 데이터
	TMap<FItemMetaInfo, uint32> CurrentRemainItemMetaValue;

	UPROPERTY(EditDefaultsOnly, Category = "Options",
		meta = (AllowPrivateAccess = true))
	uint8 InventorySlotCount = 54;

	UPROPERTY()
	TArray<FItemMetaInfo> InventoryList;
};
