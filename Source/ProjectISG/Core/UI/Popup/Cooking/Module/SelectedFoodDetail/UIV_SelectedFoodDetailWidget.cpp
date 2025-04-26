#include "UIV_SelectedFoodDetailWidget.h"

#include "Components/ScrollBox.h"
#include "ProjectISG/Core/UI/Popup/Cooking/Module/FoodIngredientInfo/UIV_FoodIngredientInfoWidget.h"

void UUIV_SelectedFoodDetailWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (FoodIngredientInfoWidgetClass)
	{
		for (int i = 0; i < DebugFoodIngredientInfoCount; i++)
		{
			IngredientListScroll->AddChild(
				CreateWidget(this, FoodIngredientInfoWidgetClass));
		}
	}
}
