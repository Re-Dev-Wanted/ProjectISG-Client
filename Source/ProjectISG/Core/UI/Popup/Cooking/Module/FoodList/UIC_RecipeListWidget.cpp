#include "UIC_RecipeListWidget.h"

#include "UIV_RecipeListWidget.h"
#include "Components/GridPanel.h"
#include "ProjectISG/Contents/Cooking/Managers/CookingManager.h"
#include "ProjectISG/Core/UI/Popup/Cooking/Module/FoodInfo/UIC_FoodInfoWidget.h"
#include "ProjectISG/Core/UI/Popup/Cooking/Module/FoodInfo/UIV_FoodInfoWidget.h"

void UUIC_RecipeListWidget::InitializeData()
{
	UUIV_RecipeListWidget* RecipeListWidgetView = Cast<UUIV_RecipeListWidget>(
		GetView());

	RecipeListWidgetView->GetRecipeListGrid()->ClearChildren();

	int i = 0;
	for (auto RecipeData :
	     UCookingManager::GetRecipeData())
	{
		UUIV_FoodInfoWidget* NewWidget = CreateWidget<UUIV_FoodInfoWidget>(
			RecipeListWidgetView,
			RecipeListWidgetView->GetFoodInfoWidgetClass());

		RecipeListWidgetView->GetRecipeListGrid()->AddChildToGrid(
			NewWidget, i / RecipeListWidgetView->GetColumnValue()
			, i % RecipeListWidgetView->GetColumnValue());

		Cast<UUIC_FoodInfoWidget>(NewWidget->GetController())->SetFoodThumbnail(
			RecipeData.Value.GetFoodId());

		i += 1;
	}
}
