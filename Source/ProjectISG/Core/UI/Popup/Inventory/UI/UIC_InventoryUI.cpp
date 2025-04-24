#include "UIC_InventoryUI.h"
#include "UIV_InventoryUI.h"

#include "EnhancedInputComponent.h"
#include "ProjectISG/Core/UI/HUD/Inventory/InventoryList.h"

void UUIC_InventoryUI::UpdateMainSlotItemData() const
{
	const UUIV_InventoryUI* InventoryUIView = Cast<UUIV_InventoryUI>(GetView());
	InventoryUIView->GetMainSlotList()->UpdateItemData();
}

void UUIC_InventoryUI::UpdateInventorySlotItemData() const
{
	const UUIV_InventoryUI* InventoryUIView = Cast<UUIV_InventoryUI>(GetView());
	InventoryUIView->GetInventoryList()->UpdateItemData();
}

void UUIC_InventoryUI::BindInputAction(UEnhancedInputComponent* InputComponent)
{
	Super::BindInputAction(InputComponent);

	InputComponent->BindAction(CloseInventoryAction, ETriggerEvent::Triggered,
	                           this, &ThisClass::PopUIFromPlayerController);
}
