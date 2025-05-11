#include "UIC_MainHUD.h"

#include "UIV_MainHUD.h"
#include "ProjectISG/Core/UI/UIEnum.h"
#include "ProjectISG/Core/UI/HUD/Inventory/InventoryList.h"
#include "ProjectISG/Core/UI/HUD/Interactive/InteractiveUI.h"

void UUIC_MainHUD::AppearUI(const EUILayer Layer)
{
	Super::AppearUI(Layer);

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

	MainHUDView->GetInteractiveUI()->ClearItems();

	MainHUDView->GetInteractiveUI()->SetInteractive(IsShow);
}

void UUIC_MainHUD::ToggleInteractiveUI(const FString& DisplayKey,
                                       const FString& DisplayText)
{
	const UUIV_MainHUD* MainHUDView = Cast<UUIV_MainHUD>(GetView());

	MainHUDView->GetInteractiveUI()->SetInteractive(true);
	MainHUDView->GetInteractiveUI()->SetInteractive(DisplayKey, DisplayText);
}

void UUIC_MainHUD::AdditiveToggleInteractiveUI(const FString& DisplayKey,
	const FString& DisplayText)
{
	const UUIV_MainHUD* MainHUDView = Cast<UUIV_MainHUD>(GetView());
	
	if (MainHUDView->GetInteractiveUI()->IsInteractiveHidden())
	{
		return;
	}

	MainHUDView->GetInteractiveUI()->AddInteractive(DisplayKey, DisplayText);
}

void UUIC_MainHUD::TogglePlacementIndicatorUI(const bool Visible)
{
	const UUIV_MainHUD* MainHUDView = Cast<UUIV_MainHUD>(GetView());

	MainHUDView->GetInteractiveUI()->SetPlacementIndicator(Visible);
}
