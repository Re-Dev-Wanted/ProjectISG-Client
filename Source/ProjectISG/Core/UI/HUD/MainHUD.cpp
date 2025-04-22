#include "MainHUD.h"

#include "Interactive/InteractiveUI.h"
#include "Inventory/InventoryList.h"

void UMainHUD::InitializeHUD()
{
	MainSlotList->SelectSlot(0, 0);
}

void UMainHUD::ToggleInteractiveUI(const bool IsShow)
{
	InteractiveUI->SetVisibility(IsShow
		                             ? ESlateVisibility::SelfHitTestInvisible
		                             : ESlateVisibility::Hidden);
}

void UMainHUD::ToggleInteractiveUI(const FString& DisplayKey,
                                   const FString& DisplayText)
{
	InteractiveUI->SetInteractive(DisplayKey, DisplayText);
	InteractiveUI->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}
