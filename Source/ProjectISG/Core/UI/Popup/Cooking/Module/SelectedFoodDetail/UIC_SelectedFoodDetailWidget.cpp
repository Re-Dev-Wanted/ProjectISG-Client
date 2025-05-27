#include "UIC_SelectedFoodDetailWidget.h"

#include "UIV_SelectedFoodDetailWidget.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "ProjectISG/Contents/Cooking/Managers/CookingManager.h"
#include "ProjectISG/Core/UI/Popup/Cooking/Module/FoodIngredientInfo/UIC_FoodIngredientInfoWidget.h"
#include "ProjectISG/Core/UI/Popup/Cooking/Module/FoodIngredientInfo/UIV_FoodIngredientInfoWidget.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"

void UUIC_SelectedFoodDetailWidget::ShowDataByRecipeId(const uint32 RecipeId)
{
	const FFoodRecipe Recipe = UCookingManager::GetRecipeData()[RecipeId];
	const FItemInfoData FoodInfo = UItemManager::GetItemInfoById(
		Recipe.GetFoodId());

	const UUIV_SelectedFoodDetailWidget* SelectedFoodDetailView = Cast<
		UUIV_SelectedFoodDetailWidget>(GetView());

	SelectedFoodDetailView->GetSelectedFoodName()->SetText(
		FText::FromString(FoodInfo.GetDisplayName()));

	UTexture2D* LoadedTexture = FoodInfo.GetThumbnail().LoadSynchronous();
	
	SelectedFoodDetailView->GetItemIcon()->SetBrushFromTexture(LoadedTexture);

	SelectedFoodDetailView->GetFoodDesc()->SetText(FText::FromString(FoodInfo.GetDescription()));

	SelectedFoodDetailView->GetIngredientListScroll()->ClearChildren();
	
	for (const auto RecipeData : Recipe.GetRecipeData())
	{
		UUIV_FoodIngredientInfoWidget* NewWidget = CreateWidget<
			UUIV_FoodIngredientInfoWidget>(GetView()
											, SelectedFoodDetailView->
											GetFoodIngredientInfoWidgetClass());

		SelectedFoodDetailView->GetIngredientListScroll()->AddChild(NewWidget);
		Cast<UUIC_FoodIngredientInfoWidget>(NewWidget->GetController())->
			SetIngredientInfo(RecipeData.Key, RecipeData.Value);
	}
}
