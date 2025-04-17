#include "InventoryList.h"

#include "Components/GridPanel.h"
#include "Module/InventorySlot.h"

#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"

void UInventoryList::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (!InventorySlotClass)
	{
		return;
	}

	InventoryList->ClearChildren();
	for (int i = 0; i < ListRow * ListColumn; i++)
	{
		UInventorySlot* NewSlot = CreateWidget<UInventorySlot>(
			this, InventorySlotClass);
		NewSlot->SetPadding(4);

		InventoryList->AddChildToGrid(NewSlot, i / ListColumn, i % ListColumn);
	}
}

void UInventoryList::NativeConstruct()
{
	Super::NativeConstruct();

	InventoryList->ClearChildren();
	for (int i = StartIndex; i < ListRow * ListColumn + StartIndex; i++)
	{
		UInventorySlot* NewSlot = CreateWidget<UInventorySlot>(
			this, InventorySlotClass);
		NewSlot->SetPadding(4);

		InventoryList->AddChildToGrid(NewSlot, (i - StartIndex) / ListColumn,
		                              (i - StartIndex) % ListColumn);

		FItemMetaInfo ItemMetaInfo = GetOwningPlayerState<AMainPlayerState>()->
		                             GetInventoryComponent()->GetInventoryList()
			[i];
		NewSlot->SetIndex(i);
		NewSlot->SetSlotInfo(ItemMetaInfo);
	}
}
