#include "InventorySlot.h"

#include "InventorySlotDragDropOperation.h"
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
#include "ProjectISG/Core/UI/Popup/Inventory/UI/UIC_InventoryUI.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"

FReply UInventorySlot::NativeOnMouseButtonDown(const FGeometry& InGeometry,
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

void UInventorySlot::NativeOnDragDetected(const FGeometry& InGeometry,
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
	DragItemWidget = CreateWidget<UInventorySlot>(this, DragItemWidgetClass);
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

	if (OnInventorySlotDragDetected.IsBound())
	{
		OnInventorySlotDragDetected.Broadcast(SlotItemId);
	}
}

bool UInventorySlot::NativeOnDrop(const FGeometry& InGeometry,
                                  const FDragDropEvent& InDragDropEvent,
                                  UDragDropOperation* InOperation)
{
	if (IsDragUi)
	{
		return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	}
	// 드래그 필요한 UI인 경우 여기에 접근하는 순간부터 우선은 true로 교체해준다.
	IsExecuteDrop = true;

	// 이 경우가 애초부터 인벤토리 칸과 인벤토리 칸과의 교차임을 의미한다.
	// 근데 이후 불안한 경우 내부에서 Operation의 Widget이 InvenSlot인지 확인
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

	UInventorySlot* PrevInvSlot = Cast<UInventorySlot>(DropOperation->
		GetOriginWidget());

	const AMainPlayerState* PS = Cast<AMainPlayerState>(
		GetOwningPlayerPawn()->GetPlayerState());

	TArray<FItemMetaInfo> InventoryList = PS->GetInventoryComponent()->
	                                          GetInventoryList();
	SetSlotInfo(InventoryList[DropOperation->GetItemIndex()]);
	PrevInvSlot->SetSlotInfo(InventoryList[Index]);

	PS->GetInventoryComponent()->SwapItemInInventory(
		DropOperation->GetItemIndex(), Index);

	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

void UInventorySlot::NativeOnDragCancelled(
	const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
}

void UInventorySlot::SetSlotInfo(const FItemMetaInfo& ItemMetaInfo)
{
	SetSlotItemId(ItemMetaInfo.GetId());
	SetThumbnail(
		UItemManager::GetItemInfoById(ItemMetaInfo.GetId()).GetThumbnail());
	SetItemCount(ItemMetaInfo.GetCurrentCount());
}

void UInventorySlot::SetIsDragged(const bool IsDragged)
{
	ItemOverlay->SetRenderOpacity(IsDragged ? 0.5 : 1);
}

void UInventorySlot::SetThumbnail(
	const TSoftObjectPtr<UTexture2D>& Thumbnail) const
{
	// 처음 로딩 시 반드시 동기적으로 로딩을 해줘야 한다.
	UTexture2D* LoadedTexture = Thumbnail.LoadSynchronous();
	ItemThumbnail.Get()->SetBrushFromTexture(LoadedTexture);
}

void UInventorySlot::SetItemCount(const uint16 NewCount) const
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

void UInventorySlot::SetSelected(const bool IsSelected) const
{
	SelectedBorder->SetVisibility(IsSelected
		                              ? ESlateVisibility::Visible
		                              : ESlateVisibility::Hidden);
}

void UInventorySlot::NativeOnMouseEnter(const FGeometry& InGeometry,
                                        const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);


	if (UBaseUIController* UIController = GetOwningPlayer<
			AMainPlayerController>()->GetUIManageComponent()->
			                          ControllerInstances.FindRef(
				                          EUIName::Popup_InventoryUI))
	{
		Cast<UUIC_InventoryUI>(UIController)->SetItemInfoData(Index);
	}

	// TODO: 다른 전용 UI 필요하면 아래에 계속 적어도 무방
}

void UInventorySlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	UUIC_InventoryUI* InventoryUIController = Cast<UUIC_InventoryUI>(
		GetOwningPlayer<AMainPlayerController>()->GetUIManageComponent()->
		                                          ControllerInstances[
			EUIName::Popup_InventoryUI]);
	InventoryUIController->ClearItemInfoData();

	// TODO: 다른 전용 UI 필요하면 아래에 계속 적어도 무방
}
