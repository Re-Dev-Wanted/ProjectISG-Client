#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProjectISG/Systems/Inventory/ItemData.h"
#include "InventorySlot.generated.h"

class UImage;
class UBorder;
class UOverlay;
class UTextBlock;

UCLASS()
class PROJECTISG_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()

public:
	void RemoveDragDropSlot() const;

protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry,
	                                       const FPointerEvent&
	                                       InMouseEvent) override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry,
	                                  const FPointerEvent& InMouseEvent,
	                                  UDragDropOperation*&
	                                  OutOperation) override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry,
	                          const FDragDropEvent& InDragDropEvent,
	                          UDragDropOperation* InOperation) override;

	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent,
	                                   UDragDropOperation*
	                                   InOperation) override;

private:
	bool IsDragUi;
	bool IsExecuteDrop = true;

	uint16 Index;
	// 해당 슬롯이 표현하는 아이템의 아이디
	uint16 SlotItemId;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<UInventorySlot> DragItemWidgetClass;

	UPROPERTY()
	TObjectPtr<UInventorySlot> DragItemWidget;

	FVector2d DragStartOffset;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> ItemOverlay;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemThumbnail;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemCount;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> SelectedBorder;

	void SetSlotInfo(const FItemMetaInfo& ItemMetaInfo);

	void SetIsDragged(const bool IsDragged);
};
