#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "UIV_CookingRecipeUI.generated.h"

class UUIV_SelectedFoodDetailWidget;
class UUIV_RecipeListWidget;
class UButton;
class UScrollBox;

UCLASS()
class PROJECTISG_API UUIV_CookingRecipeUI : public UBaseUIView
{
	GENERATED_BODY()

public:
	GETTER(TObjectPtr<UUIV_SelectedFoodDetailWidget>, SelectedFoodDetail)
	GETTER(TObjectPtr<UButton>, CookingButton)

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> RecipeListScroll;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUIV_RecipeListWidget> RecipeList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CookingButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUIV_SelectedFoodDetailWidget> SelectedFoodDetail;
};
