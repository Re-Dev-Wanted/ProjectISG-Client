#include "LootContainerInventoryList.h"

#include "Components/GridPanel.h"

#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Core/UI/HUD/Inventory/Module/InventorySlot.h"
#include "ProjectISG/Core/UI/Popup/LootContainer/UI/Widgets/LootContainerItemSlot.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"

void ULootContainerInventoryList::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (!InventorySlotClass)
	{
		return;
	}

	InventoryList->ClearChildren();
	for (int i = 0; i < ListRow * ListColumn; i++)
	{
		ULootContainerItemSlot* NewSlot = CreateWidget<ULootContainerItemSlot>(
			this, InventorySlotClass);
		
		// NewSlot->SetPadding(4);

		InventoryList->AddChildToGrid(NewSlot, i / ListColumn, i % ListColumn);
	}
}

void ULootContainerInventoryList::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateItemData();
}

void ULootContainerInventoryList::UpdateItemData()
{
	InventoryList->ClearChildren();
	for (int i = StartIndex; i < ListRow * ListColumn + StartIndex; i++)
	{
		ULootContainerItemSlot* NewSlot = CreateWidget<ULootContainerItemSlot>(
			this, InventorySlotClass);
		// NewSlot->SetPadding(4);

		InventoryList->AddChildToGrid(NewSlot, (i - StartIndex) / ListColumn,
									  (i - StartIndex) % ListColumn);

		FItemMetaInfo ItemMetaInfo = GetOwningPlayerState<AMainPlayerState>()->
									 GetInventoryComponent()->GetInventoryList()
			[i];
		NewSlot->SetItemHandler(GetOwningPlayerState<AMainPlayerState>()->
									 GetInventoryComponent());
		NewSlot->SetIndex(i);
		NewSlot->SetSlotInfo(ItemMetaInfo);
	}
}

void ULootContainerInventoryList::SelectSlot(const uint16 Prev, const uint16 Next)
{
	const ULootContainerItemSlot* PrevSelectedSlot = Cast<ULootContainerItemSlot>(
		InventoryList->GetChildAt(Prev));
	PrevSelectedSlot->SetSelected(false);

	const ULootContainerItemSlot* NextSelectedSlot = Cast<ULootContainerItemSlot>(
		InventoryList->GetChildAt(Next));
	NextSelectedSlot->SetSelected(true);
}