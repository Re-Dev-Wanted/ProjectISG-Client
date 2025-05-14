// Fill out your copyright notice in the Description page of Project Settings.


#include "UIV_LootContainerUI.h"

#include "Components/TextBlock.h"
#include "Views/LootContainerListView.h"

void UUIV_LootContainerUI::Construct(FGuid Guid, int32 Capacity)
{
	GuidText->SetText(FText::FromString(Guid.ToString(EGuidFormats::Digits)));
	ContainerView->Construct(InventorySlotClass, Capacity);
}
