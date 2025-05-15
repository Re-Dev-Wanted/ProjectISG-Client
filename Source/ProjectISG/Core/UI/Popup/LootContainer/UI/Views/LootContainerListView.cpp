#include "LootContainerListView.h"

#include "Components/WrapBox.h"
#include "ProjectISG/Core/UI/Popup/LootContainer/UI/Widgets/LootContainerItemSlot.h"
#include "ProjectISG/Systems/Inventory/ItemData.h"
#include "ProjectISG/Systems/LootContainer/LootContainerSubsystem.h"

void ULootContainerListView::SetContainerInfo(const TSubclassOf<ULootContainerItemSlot>& InventorySlotClass, TArray<FItemMetaInfo> Items)
{
	if (!InventorySlotClass)
	{
		return;
	}

	WrapBox->ClearChildren();

	int32 SlotCount = Items.Num();

	for (int32 i = 0; i < SlotCount; i++)
	{
		ULootContainerItemSlot* NewSlot = CreateWidget<ULootContainerItemSlot>(this, InventorySlotClass);
		WrapBox->AddChildToWrapBox(NewSlot);
		NewSlot->SetItemHandler(GetWorld()->GetGameInstance()->GetSubsystem<ULootContainerSubsystem>());
		NewSlot->SetIndex(i);
		NewSlot->SetSlotInfo(Items[i]);
	}
}
