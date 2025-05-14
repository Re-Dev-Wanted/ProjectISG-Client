#include "LootContainerListView.h"

#include "Components/WrapBox.h"
#include "ProjectISG/Core/UI/HUD/Inventory/Module/InventorySlot.h"

void ULootContainerListView::Construct(TSubclassOf<UInventorySlot> 
InventorySlotClass, int32 SlotCount)
{
	if (!InventorySlotClass)
	{
		return;
	}

	WrapBox->ClearChildren();

	for (int32 i = 0; i < SlotCount; i++)
	{
		UInventorySlot* NewSlot = CreateWidget<UInventorySlot>(this, InventorySlotClass);
		WrapBox->AddChildToWrapBox(NewSlot);
		NewSlot->SetIndex(i);
	}
}
