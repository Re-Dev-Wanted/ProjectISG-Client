#include "LootContainerItemSlot.h"

#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Border.h"
#include "Components/Overlay.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetInputLibrary.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"

#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Core/UI/Base/Components/UIManageComponent.h"
#include "ProjectISG/Core/UI/HUD/Inventory/Module/InventorySlotDragDropOperation.h"
#include "ProjectISG/Core/UI/Popup/Inventory/UI/UIC_InventoryUI.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"

FReply ULootContainerItemSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry,
                                               const FPointerEvent&
                                               InMouseEvent)
{
	if (IsDragUi)
	{
		Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	}

	DragStartOffset = USlateBlueprintLibrary::AbsoluteToLocal(InGeometry,
		UKismetInputLibrary::PointerEvent_GetScreenSpacePosition(InMouseEvent));

	return UWidgetBlueprintLibrary::DetectDragIfPressed(
		InMouseEvent,
		this,
		EKeys::LeftMouseButton
	).NativeReply;
}

void ULootContainerItemSlot::NativeOnDragDetected(const FGeometry& InGeometry,
                                          const FPointerEvent& InMouseEvent,
                                          UDragDropOperation*& OutOperation)
{
	if (IsDragUi)
	{
		return;
	}

	if (SlotItemId == 0)
	{
		return;
	}

	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	// 드래그를 위한 위젯 설정
	DragItemWidget = CreateWidget<ULootContainerItemSlot>(this, DragItemWidgetClass);
	DragItemWidget->SetThumbnail(ItemThumbnail->GetBrush().GetResourceObject());
	DragItemWidget->ItemCount->SetText(ItemCount->GetText());
	DragItemWidget->SetIsDragged(true);

	// 드래그 드롭 관련 오퍼레이션 설정
	OutOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(
		UInventorySlotDragDropOperation::StaticClass());
	OutOperation->Pivot = EDragPivot::MouseDown;
	OutOperation->DefaultDragVisual = DragItemWidget;

	if (OutOperation->IsA(UInventorySlotDragDropOperation::StaticClass()))
	{
		UInventorySlotDragDropOperation* DragDropOperation = Cast<
			UInventorySlotDragDropOperation>(OutOperation);

		DragDropOperation->SetOriginWidget(this);
		DragDropOperation->SetItemIndex(Index);
	}
}

bool ULootContainerItemSlot::NativeOnDrop(const FGeometry& InGeometry,
                                  const FDragDropEvent& InDragDropEvent,
                                  UDragDropOperation* InOperation)
{
	if (IsDragUi)
	{
		return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	}

	IsExecuteDrop = true;
	
	if (!InOperation->IsA(UInventorySlotDragDropOperation::StaticClass()))
	{
		return false;
	}

	const UInventorySlotDragDropOperation* DropOperation
		= Cast<UInventorySlotDragDropOperation>(InOperation);

	if (!DropOperation->GetOriginWidget().IsA(StaticClass()))
	{
		return false;
	}

	ULootContainerItemSlot* PrevInvSlot = Cast<ULootContainerItemSlot>(DropOperation->
		GetOriginWidget());

	TObjectPtr<UObject> PrevObj = PrevInvSlot->ItemHandler.GetObjectRef();
	TObjectPtr<UObject> CurrentObj = ItemHandler.GetObjectRef();

	const FItemMetaInfo CurrentItemInfo = ItemHandler->GetItemMetaInfo(ContainerGuid, Index);
	const FItemMetaInfo PrevItemInfo = PrevInvSlot->ItemHandler->GetItemMetaInfo(PrevInvSlot->ContainerGuid, PrevInvSlot->Index);

	if (PrevObj.GetClass() == CurrentObj.GetClass())
	{
		ItemHandler->SwapItem(ContainerGuid, PrevInvSlot->Index, Index);
	}
	else
	{
		PrevInvSlot->ItemHandler->ChangeItem(PrevInvSlot->ContainerGuid, CurrentItemInfo, PrevInvSlot->Index);
		ItemHandler->ChangeItem(ContainerGuid, PrevItemInfo, Index);
	}
	
	SetSlotInfo(PrevItemInfo, ContainerGuid);
	PrevInvSlot->SetSlotInfo(CurrentItemInfo, PrevInvSlot->ContainerGuid);

	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

void ULootContainerItemSlot::NativeOnDragCancelled(
	const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
}

void ULootContainerItemSlot::SetSlotInfo(const FItemMetaInfo& ItemMetaInfo, const FGuid& Guid)
{
	SetSlotItemId(ItemMetaInfo.GetId());
	SetThumbnail(
		UItemManager::GetItemInfoById(ItemMetaInfo.GetId()).GetThumbnail());
	SetItemCount(ItemMetaInfo.GetCurrentCount());

	ContainerGuid = Guid;
}

void ULootContainerItemSlot::SetIsDragged(const bool IsDragged)
{
	ItemOverlay->SetRenderOpacity(IsDragged ? 0.5 : 1);
}

void ULootContainerItemSlot::SetThumbnail(
	const TSoftObjectPtr<UTexture2D>& Thumbnail) const
{
	// 처음 로딩 시 반드시 동기적으로 로딩을 해줘야 한다.
	UTexture2D* LoadedTexture = Thumbnail.LoadSynchronous();
	ItemThumbnail.Get()->SetBrushFromTexture(LoadedTexture);
}

void ULootContainerItemSlot::SetItemCount(const uint16 NewCount) const
{
	if (NewCount > 1)
	{
		ItemCount->SetText(FText::AsNumber(NewCount));
	}
	else
	{
		ItemCount->SetText(FText::GetEmpty());
	}
}

void ULootContainerItemSlot::SetSelected(const bool IsSelected) const
{
	SelectedBorder->SetVisibility(IsSelected
		                              ? ESlateVisibility::Visible
		                              : ESlateVisibility::Hidden);
}

void ULootContainerItemSlot::NativeOnMouseEnter(const FGeometry& InGeometry,
                                        const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	const auto ControllerInstances = GetOwningPlayer<AMainPlayerController>()->
	                                 GetUIManageComponent()->
	                                 ControllerInstances;

	if (ControllerInstances.Contains(EUIName::Popup_InventoryUI))
	{
		Cast<UUIC_InventoryUI>(
				ControllerInstances[EUIName::Popup_InventoryUI])->
			SetItemInfoData(Index);
	}
	
}

void ULootContainerItemSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	const auto ControllerInstances = GetOwningPlayer<AMainPlayerController>()->
	                                 GetUIManageComponent()->
	                                 ControllerInstances;

	if (ControllerInstances.Contains(EUIName::Popup_InventoryUI))
	{
		Cast<UUIC_InventoryUI>(
				ControllerInstances[EUIName::Popup_InventoryUI])->
			ClearItemInfoData();
	}
	
}