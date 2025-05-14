#include "UIC_InventoryUI.h"
#include "UIV_InventoryUI.h"

#include "EnhancedInputComponent.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Core/UI/HUD/Inventory/InventoryList.h"
#include "ProjectISG/Core/UI/HUD/Inventory/Module/ItemInfo.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"

void UUIC_InventoryUI::AppearUI()
{
	Super::AppearUI();

	ClearItemInfoData();
}

void UUIC_InventoryUI::ClearItemInfoData()
{
	const UUIV_InventoryUI* InventoryUIView = Cast<UUIV_InventoryUI>(GetView());
	InventoryUIView->GetItemInfoTooltip()->SetVisibility(
		ESlateVisibility::Hidden);
}

void UUIC_InventoryUI::SetItemInfoData(const uint8 InventoryIndex)
{
	const UUIV_InventoryUI* InventoryUIView = Cast<UUIV_InventoryUI>(GetView());
	const AMainPlayerState* PS = InventoryUIView->GetOwningPlayerState<
		AMainPlayerState>();

	const FItemMetaInfo ItemMetaInfo = PS->GetInventoryComponent()->
	                                       GetInventoryList()[InventoryIndex];

	if (ItemMetaInfo.GetId() == 0)
	{
		ClearItemInfoData();
		return;
	}

	InventoryUIView->GetItemInfoTooltip()->ShowItemData(ItemMetaInfo);
}

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
