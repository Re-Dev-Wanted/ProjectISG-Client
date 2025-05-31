#include "UIV_LootContainerUI.h"

#include "Views/LootContainerListView.h"

void UUIV_LootContainerUI::SetContainer(const TArray<FItemMetaInfo>& Items
										, TScriptInterface<IItemHandler>
										Handler) const
{
	ContainerView->SetContainerInfo(InventorySlotClass, Items, Handler);
}
