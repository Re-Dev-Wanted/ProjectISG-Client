#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProjectISG/Core/UI/Popup/Crafting/UI/UIC_WorkbenchUI.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "ItemListGroup.generated.h"

struct FCraftingRecipeUIModel;
class FOnSelectedRecipe;
class UItemInfoWidget;
class UHorizontalBox;

UCLASS()
class PROJECTISG_API UItemListGroup : public UUserWidget
{
	GENERATED_BODY()
	
public:
	GETTER(int32, Index);
	
	void SetGroup(int32 GroupIndex, const TArray<FCraftingRecipeUIModel>& Recipes, FOnSelectedCraftingRecipe OnSelectedRecipe);

protected:
	virtual void NativePreConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> ItemGroupBox;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UItemInfoWidget> WidgetFactory; 

	UPROPERTY(EditDefaultsOnly)
	int8 DebugCount = 5;

private:
	int32 Index = -1;
	
};
