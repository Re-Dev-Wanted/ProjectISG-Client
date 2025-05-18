#pragma once

#include "CoreMinimal.h"
#include "UIC_WorkbenchUI.h"
#include "ProjectISG/Core/UI/Base/Module/UI_BaseButton.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "ProjectISG/Core/UI/Popup/Crafting/Views/MaterialsView.h"
#include "UIV_WorkbenchUI.generated.h"

struct FCraftingRecipeUIModel;
struct FCraftingRecipe;
class UItemListView;
class UButton;

UCLASS()
class PROJECTISG_API UUIV_WorkbenchUI : public UBaseUIView
{
	GENERATED_BODY()

public:
	GETTER(TObjectPtr<UButton>, CloseButton)
	GETTER(TObjectPtr<UUI_BaseButton>, CraftingButton)
	
	void OnSetup(const TArray<FCraftingRecipeUIModel>& Array, FOnSelectedCraftingRecipe OnSelected);

	void OnUpdateUI(FCraftingRecipeUIModel SelectedModel);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CloseButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UItemListView> RecipeListView;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMaterialsView> MaterialsView;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUI_BaseButton> CraftingButton;
	
};
