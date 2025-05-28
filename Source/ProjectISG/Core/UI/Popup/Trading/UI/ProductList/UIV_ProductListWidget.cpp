#include "UIV_ProductListWidget.h"

#include "UIC_ProductListWidget.h"
#include "ProjectISG/Core/UI/Popup/Trading/UI/TradingTabButton.h"
#include "ProjectISG/Core/UI/Popup/Trading/UI/UIM_TradingUI.h"
#include "ProjectISG/Core/UI/Popup/Trading/UI/ProductInfo/UIV_ProductInfoWidget.h"

void UUIV_ProductListWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UUIC_ProductListWidget* ProductListWidgetController = Cast<
		UUIC_ProductListWidget>(GetController());

	ProductListWidgetController->InitializeData();
}

void UUIV_ProductListWidget::SetUpdateUI(ETradingState State)
{
	BuyTabButton->OnActive(State == ETradingState::BUY);
	SellTabButton->OnActive(State == ETradingState::SELL);

	UUIC_ProductListWidget* ProductListWidgetController = Cast<
		UUIC_ProductListWidget>(GetController());

	ProductListWidgetController->OnUpdateList(State);
}
