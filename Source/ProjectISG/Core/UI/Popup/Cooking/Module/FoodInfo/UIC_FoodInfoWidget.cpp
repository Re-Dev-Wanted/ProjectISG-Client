#include "UIC_FoodInfoWidget.h"

#include "UIM_FoodInfoWidget.h"
#include "UIV_FoodInfoWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "ProjectISG/Contents/Cooking/Managers/CookingManager.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/UI/Base/Components/UIManageComponent.h"
#include "ProjectISG/Core/UI/Popup/Cooking/UI/CookingRecipe/UIC_CookingRecipeUI.h"
#include "ProjectISG/Core/UI/Popup/Cooking/UI/CookingRecipe/UIV_CookingRecipeUI.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"

void UUIC_FoodInfoWidget::InitializeController(UBaseUIView* NewView
												, UBaseUIModel* NewModel)
{
	Super::InitializeController(NewView, NewModel);

	const UUIV_FoodInfoWidget* FoodInfoWidget = Cast<UUIV_FoodInfoWidget>(
		GetView());

	FoodInfoWidget->GetFoodSelectButton()->OnClicked.AddDynamic(
		this, &ThisClass::OnSelectFoodData);
}

void UUIC_FoodInfoWidget::SetFoodInfo(const uint32 RecipeId)
{
	const UUIV_FoodInfoWidget* FoodInfoWidget = Cast<UUIV_FoodInfoWidget>(
		GetView());
	// 레시피 Id를 저장함
	UUIM_FoodInfoWidget* FoodInfoModel = Cast<UUIM_FoodInfoWidget>(GetModel());
	FoodInfoModel->SetRecipeId(RecipeId);

	// 레시피 Id를 바탕으로 아이템 정보를 불러옴
	const uint32 FoodId = UCookingManager::GetRecipeData()[RecipeId].
		GetFoodId();
	const FItemInfoData ItemInfoData = UItemManager::GetItemInfoById(FoodId);

	// 아이템 정보를 바탕으로 View 세팅
	UTexture2D* Thumbnail = ItemInfoData.GetThumbnail().LoadSynchronous();
	FoodInfoWidget->GetFoodThumbnail()->SetBrushFromTexture(Thumbnail);
	FoodInfoWidget->GetFoodName()->SetText(
		FText::FromString(ItemInfoData.GetDisplayName()));
}

void UUIC_FoodInfoWidget::OnSelectFoodData()
{
	const UUIM_FoodInfoWidget* FoodInfoModel = Cast<UUIM_FoodInfoWidget>(
		GetModel());

	// 상위 위젯인 요리 모드 UI Controller 가져옴
	UUIC_CookingRecipeUI* CookingRecipeUIController = Cast<
		UUIC_CookingRecipeUI>(
		GetView()->GetOwningPlayer<AMainPlayerController>()->
					GetUIManageComponent()->ControllerInstances[
			EUIName::Popup_CookingRecipeUI]);

	// 상위 위젯 컨트롤러의 레시피 ID를 변경한다.
	CookingRecipeUIController->SetSelectedCookingRecipe(
		FoodInfoModel->GetRecipeId());
}
