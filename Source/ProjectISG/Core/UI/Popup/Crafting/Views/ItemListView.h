#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProjectISG/Core/UI/Popup/Crafting/UI/UIC_WorkbenchUI.h"
#include "ItemListView.generated.h"

struct FCraftingRecipeUIModel;
struct FCraftingRecipe;
class UItemListGroup;
class UScrollBox;

UCLASS()
class PROJECTISG_API UItemListView : public UUserWidget
{
	GENERATED_BODY()

public:
	void OnSetup(const TArray<FCraftingRecipeUIModel>& Recipes, FOnSelectedCraftingRecipe OnSelected);

protected:
	virtual void NativePreConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> ScrollBox;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UItemListGroup> ItemFactory;

	UPROPERTY(EditDefaultsOnly)
	int8 DebugCount = 10;

	UPROPERTY(EditDefaultsOnly)
	int32 GroupingCount = 5;
	
};
