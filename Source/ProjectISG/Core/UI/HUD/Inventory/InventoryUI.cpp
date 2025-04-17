#include "InventoryUI.h"

#include "Components/GridPanel.h"
#include "Module/InventorySlot.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"

void UInventoryUI::NativePreConstruct()
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

void UInventoryUI::NativeConstruct()
{
	Super::NativeConstruct();

	InventoryList->ClearChildren();
	for (int i = 0; i < ListRow * ListColumn; i++)
	{
		UInventorySlot* NewSlot = CreateWidget<UInventorySlot>(
			this, InventorySlotClass);
		NewSlot->SetPadding(4);

		InventoryList->AddChildToGrid(NewSlot, i / ListColumn, i % ListColumn);
		FItemMetaInfo ItemMetaInfo = GetOwningPlayerState<AMainPlayerState>()->
		                             GetInventoryComponent()->GetInventoryList()
			[i];
		NewSlot->SetIndex(i);
		NewSlot->SetSlotInfo(ItemMetaInfo);
	}
}
