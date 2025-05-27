// Fill out your copyright notice in the Description page of Project Settings.


#include "UIV_LootContainerUI.h"

#include "Components/TextBlock.h"
#include "Views/LootContainerListView.h"

void UUIV_LootContainerUI::SetContainer(FGuid Guid, const TArray<FItemMetaInfo>& Items, TScriptInterface<IItemHandler> Handler) const
{
	ContainerView->SetContainerInfo(InventorySlotClass, Items, Guid, Handler);
}
