#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectISG/Systems/Inventory/ItemData.h"
#include "ProjectISG/Utils/MacroUtil.h"

#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdateNotified);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTISG_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();

	GETTER(uint8, InventorySlotCount)
	GETTER(TArray<FItemMetaInfo>, InventoryList)

	uint32 AddItem(const FItemMetaInfo& ItemInfo);

	bool HasItemInInventory(const uint32 Id, const uint32 Count);

	bool DropItem(const uint16 Index, const uint32 Count);

	bool RemoveItem(const uint16 Id, const uint32 Count);

	uint32 AddItemToInventory(const uint16 Index,
	                          const FItemMetaInfo& ItemInfo);

	void SwapItemInInventory(const uint16 Prev, const uint16 Next);

	FORCEINLINE TMap<uint32, uint32> GetCurrentRemainItemValue() const
	{
		return CurrentRemainItemValue;
	}

	void UpdateCurrentRemainItemValue();

	FOnInventoryUpdateNotified OnInventoryUpdateNotified;

	void InitializeItemData();

	void UpdateInventory();

protected:
	virtual void BeginPlay() override;

	virtual void InitializeComponent() override;

private:
	TMap<uint32, uint32> CurrentRemainItemValue;

	UPROPERTY(EditDefaultsOnly, Category = "Options",
		meta = (AllowPrivateAccess = true))
	uint8 InventorySlotCount = 54;

	UPROPERTY()
	TArray<FItemMetaInfo> InventoryList;
};
