#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryList.generated.h"

class UGridPanel;
class UInventorySlot;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDragDetectedNotified, uint16,
                                            ItemId, uint16, SlotIndex);

UCLASS()
class PROJECTISG_API UInventoryList : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateItemData();
	void SelectSlot(const uint16 Prev, const uint16 Next);

	UFUNCTION()
	void OnDragItemDetected(uint16 ItemId, uint16 SlotIndex);

	FOnDragDetectedNotified OnDragDetectedNotified;

protected:
	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;

private:
	UPROPERTY(EditAnywhere, Category = "Options|Data",
		meta = (AllowPrivateAccess = true))
	uint8 StartIndex = 0;

	UPROPERTY(EditAnywhere, Category = "Options|UI",
		meta = (AllowPrivateAccess))
	uint8 ListRow = 9;

	UPROPERTY(EditAnywhere, Category = "Options|UI",
		meta = (AllowPrivateAccess))
	uint8 ListColumn = 5;

	UPROPERTY(EditAnywhere, Category = "Options|UI",
		meta = (AllowPrivateAccess))
	TSubclassOf<UInventorySlot> InventorySlotClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UGridPanel> InventoryList;

	void CreateItemData();
};
