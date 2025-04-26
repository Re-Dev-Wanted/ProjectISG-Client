#include "UIC_CookingRecipeUI.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "UIM_CookingRecipeUI.h"
#include "UIV_CookingRecipeUI.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Core/UI/Popup/Cooking/Module/SelectedFoodDetail/UIC_SelectedFoodDetailWidget.h"
#include "ProjectISG/Core/UI/Popup/Cooking/Module/SelectedFoodDetail/UIV_SelectedFoodDetailWidget.h"
#include "ProjectISG/GAS/Common/Tag/ISGGameplayTag.h"

void UUIC_CookingRecipeUI::AppearUI(const EUILayer Layer)
{
	Super::AppearUI(Layer);

	UUIV_CookingRecipeUI* CookingRecipeUI = Cast<UUIV_CookingRecipeUI>(
		GetView());
	const bool IsShowSelectedRecipe = Cast<UUIM_CookingRecipeUI>(GetModel())->
		GetSelectedRecipe() != INDEX_NONE;

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
