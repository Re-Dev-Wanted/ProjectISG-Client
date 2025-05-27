#include "UIV_ProductListWidget.h"

#include "UIC_ProductListWidget.h"
#include "ProjectISG/Core/UI/Popup/Trading/UI/ProductInfo/UIV_ProductInfoWidget.h"

void UUIV_ProductListWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UUIC_ProductListWidget* ProductListWidgetController = Cast<
		UUIC_ProductListWidget>(GetController());

	ProductListWidgetController->InitializeData();
}
