#include "UIC_CookingRecipeUI.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "UIM_CookingRecipeUI.h"
#include "UIV_CookingRecipeUI.h"
#include "Components/Button.h"
#include "ProjectISG/Contents/Cooking/CookingStruct.h"
#include "ProjectISG/Contents/Cooking/Managers/CookingManager.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Core/UI/Base/Module/UI_BaseButton.h"
#include "ProjectISG/Core/UI/Popup/Cooking/Module/SelectedFoodDetail/UIC_SelectedFoodDetailWidget.h"
#include "ProjectISG/Core/UI/Popup/Cooking/Module/SelectedFoodDetail/UIV_SelectedFoodDetailWidget.h"
#include "ProjectISG/GAS/Common/Tag/ISGGameplayTag.h"
#include "ProjectISG/Systems/Inventory/ItemData.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"

void UUIC_CookingRecipeUI::AppearUI()
{
	Super::AppearUI();

	const UUIV_CookingRecipeUI* CookingRecipeUI = Cast<UUIV_CookingRecipeUI>(
		GetView());
	const bool IsShowSelectedRecipe = Cast<UUIM_CookingRecipeUI>(GetModel())->
		GetSelectedRecipe() != INDEX_NONE;

	if (!CookingRecipeUI->GetCookingButton()->Get()->OnClicked.IsBound())
	{
		CookingRecipeUI->GetCookingButton()->Get()->OnClicked.AddDynamic(
			this, &ThisClass::StartCooking);
	}

	CookingRecipeUI->GetSelectedFoodDetail()->SetVisibility(
		IsShowSelectedRecipe
			? ESlateVisibility::Visible
			: ESlateVisibility::Hidden);
}

void UUIC_CookingRecipeUI::SetSelectedCookingRecipe(const uint32 RecipeId)
{
	const UUIV_CookingRecipeUI* CookingRecipeUI = Cast<UUIV_CookingRecipeUI>(
		GetView());

	Cast<UUIM_CookingRecipeUI>(GetModel())->SetSelectedRecipe(RecipeId);
	CookingRecipeUI->GetSelectedFoodDetail()->SetVisibility(
		ESlateVisibility::Visible);

	Cast<UUIC_SelectedFoodDetailWidget>(
			CookingRecipeUI->GetSelectedFoodDetail()->GetController())->
		ShowDataByRecipeId(RecipeId);

	// 현재 레시피 기준으로 요리 가능한 지 여부를 체크해 버튼 활성화, 비활성화를 수행
	CookingRecipeUI->GetCookingButton()->Get()->SetIsEnabled(
		CanCookByRecipeId(RecipeId));
}

void UUIC_CookingRecipeUI::TurnOffSelectedCookingRecipe()
{
	const UUIV_CookingRecipeUI* CookingRecipeUI = Cast<UUIV_CookingRecipeUI>(
		GetView());

	Cast<UUIM_CookingRecipeUI>(GetModel())->SetSelectedRecipe(INDEX_NONE);
	CookingRecipeUI->GetSelectedFoodDetail()->SetVisibility(
		ESlateVisibility::Hidden);
}

void UUIC_CookingRecipeUI::BindInputAction(
	UEnhancedInputComponent* InputComponent)
{
	Super::BindInputAction(InputComponent);

	InputComponent->BindAction(CloseCookingRecipeUI, ETriggerEvent::Triggered
	                           , this, &ThisClass::OnCloseCookingRecipeUI);
}

void UUIC_CookingRecipeUI::OnCloseCookingRecipeUI()
{
	FGameplayTagContainer ActivateTag;
	ActivateTag.AddTag(ISGGameplayTags::Cooking_Active_EndCooking);

	GetPlayerController()->GetPlayerState<AMainPlayerState>()->
	                       GetAbilitySystemComponent()->
	                       TryActivateAbilitiesByTag(ActivateTag);
}

void UUIC_CookingRecipeUI::StartCooking()
{
	FGameplayTagContainer ActivateTag;
	ActivateTag.AddTag(ISGGameplayTags::Cooking_Active_QTEAction);

	GetPlayerController()->GetPlayerState<AMainPlayerState>()->
	                       GetAbilitySystemComponent()->
	                       TryActivateAbilitiesByTag(ActivateTag);
}

bool UUIC_CookingRecipeUI::CanCookByRecipeId(const uint32 RecipeId)
{
	const FFoodRecipe Recipe = UCookingManager::GetRecipeData()[RecipeId];
	const FItemInfoData FoodInfo = UItemManager::GetItemInfoById(
		Recipe.GetFoodId());

	const UInventoryComponent* InventoryComponent = GetView()->
	                                                GetOwningPlayerState<
		                                                AMainPlayerState>()->
	                                                GetInventoryComponent();

	for (const auto [IngredientId, IngredientCount] : Recipe.GetRecipeData())
	{
		// 특정 재료 자체가 인벤토리에 존재하지 않는 경우
		if (!InventoryComponent->GetCurrentRemainItemValue().Contains(
			IngredientId))
		{
			return false;
		}

		// 특정 재료의 갯수가 요구 갯수 보다 적은 경우
		if (InventoryComponent->GetCurrentRemainItemValue()[IngredientId] <
			IngredientCount)
		{
			return false;
		}
	}

	return true;
}
