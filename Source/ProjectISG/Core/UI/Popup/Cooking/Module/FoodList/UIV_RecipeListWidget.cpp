#include "UIV_RecipeListWidget.h"

#include "UIC_RecipeListWidget.h"
#include "Components/GridPanel.h"
#include "ProjectISG/Core/UI/Popup/Cooking/Module/FoodInfo/UIV_FoodInfoWidget.h"

void UUIV_RecipeListWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (FoodInfoWidgetClass)
	{
		for (int i = 0; i < DebugRecipeCount; i++)
		{
			UUIV_FoodInfoWidget* FoodInfoWidget = CreateWidget<
				UUIV_FoodInfoWidget>(this, FoodInfoWidgetClass);

			RecipeListGrid->AddChildToGrid(FoodInfoWidget,
			                               i / ColumnValue,
			                               i % ColumnValue);
		}
	}
}

void UUIV_RecipeListWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UUIC_RecipeListWidget* RecipeListWidgetController = Cast<
		UUIC_RecipeListWidget>(GetController());

	RecipeListWidgetController->InitializeData();
}
