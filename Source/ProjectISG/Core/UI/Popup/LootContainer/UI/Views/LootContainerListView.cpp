#include "LootContainerListView.h"

#include "Components/WrapBox.h"
#include "ProjectISG/Core/UI/Popup/LootContainer/UI/Widgets/LootContainerItemSlot.h"


void ULootContainerListView::Construct(const TSubclassOf<ULootContainerItemSlot>& InventorySlotClass, int32 SlotCount)
{
	if (!InventorySlotClass)
	{
		return;
	}

	WrapBox->ClearChildren();

	for (int32 i = 0; i < SlotCount; i++)
	{
		ULootContainerItemSlot* NewSlot = CreateWidget<ULootContainerItemSlot>(this, InventorySlotClass);
		WrapBox->AddChildToWrapBox(NewSlot);
		NewSlot->SetIndex(i);
	}
}
