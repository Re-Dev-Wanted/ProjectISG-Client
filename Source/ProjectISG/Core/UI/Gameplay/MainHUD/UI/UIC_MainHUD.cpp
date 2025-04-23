#include "UIC_MainHUD.h"

#include "UIV_MainHUD.h"
#include "ProjectISG/Core/UI/HUD/Inventory/InventoryList.h"
#include "ProjectISG/Core/UI/HUD/Interactive/InteractiveUI.h"

void UUIC_MainHUD::AppearUI()
{
	Super::AppearUI();

	const UUIV_MainHUD* MainHUDView = Cast<UUIV_MainHUD>(GetView());
	MainHUDView->GetMainSlotList()->UpdateItemData();
}

void UUIC_MainHUD::UpdateMainHotSlot() const
{
	const UUIV_MainHUD* MainHUDView = Cast<UUIV_MainHUD>(GetView());
	MainHUDView->GetMainSlotList()->UpdateItemData();
}

void UUIC_MainHUD::SelectSlot(const uint8 CurrentSlot,
                              const uint8 NextSlot) const
{
	const UUIV_MainHUD* MainHUDView = Cast<UUIV_MainHUD>(GetView());
	MainHUDView->GetMainSlotList()->SelectSlot(CurrentSlot, NextSlot);
}

void UUIC_MainHUD::InitializeHUD()
{
	const UUIV_MainHUD* MainHUDView = Cast<UUIV_MainHUD>(GetView());
}

void UUIC_MainHUD::ToggleInteractiveUI(const bool IsShow)
{
	const UUIV_MainHUD* MainHUDView = Cast<UUIV_MainHUD>(GetView());

	MainHUDView->GetInteractiveUI()->SetVisibility(IsShow
		                                               ? ESlateVisibility::SelfHitTestInvisible
		                                               : ESlateVisibility::Hidden);
}

void UUIC_MainHUD::ToggleInteractiveUI(const FString& DisplayKey,
                                       const FString& DisplayText)
{
	const UUIV_MainHUD* MainHUDView = Cast<UUIV_MainHUD>(GetView());

	MainHUDView->GetInteractiveUI()->SetInteractive(DisplayKey, DisplayText);
	MainHUDView->GetInteractiveUI()->SetVisibility(
		ESlateVisibility::SelfHitTestInvisible);
}
