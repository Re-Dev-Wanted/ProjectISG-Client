// Fill out your copyright notice in the Description page of Project Settings.


#include "UIV_LootContainerUI.h"

#include "Components/TextBlock.h"
#include "Views/LootContainerListView.h"

void UUIV_LootContainerUI::SetContainer(FGuid Guid, const TArray<FItemMetaInfo>& Items) const
{
	GuidText->SetText(FText::FromString(Guid.ToString(EGuidFormats::Digits)));
	ContainerView->SetContainerInfo(InventorySlotClass, Items, Guid);
}
