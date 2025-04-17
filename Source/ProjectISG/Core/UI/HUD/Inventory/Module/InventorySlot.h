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
	SETTER(uint16, Index);
	void SetSlotInfo(const FItemMetaInfo& ItemMetaInfo);
	void SetSelected(const bool IsSelected) const;

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

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry,
	                                const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

private:
	bool IsDragUi;
	bool IsExecuteDrop = true;

	uint16 Index;
	// 해당 슬롯이 표현하는 아이템의 아이디
	uint16 SlotItemId;
	SETTER(uint16, SlotItemId)

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

	void SetIsDragged(const bool IsDragged);

	void SetThumbnail(const TSoftObjectPtr<UTexture2D>& Thumbnail) const;

	void SetItemCount(const uint16 NewCount) const;
};
