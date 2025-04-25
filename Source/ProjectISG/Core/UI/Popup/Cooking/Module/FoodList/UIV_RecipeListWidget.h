#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "UIV_RecipeListWidget.generated.h"

class UUIV_FoodInfoWidget;
class UGridPanel;

UCLASS()
class PROJECTISG_API UUIV_RecipeListWidget : public UBaseUIView
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UGridPanel> RecipeListGrid;

	UPROPERTY(EditDefaultsOnly, Category = "Options",
		meta = (AllowPrivateAccess = true))
	uint16 DebugRecipeCount;

	UPROPERTY(EditAnywhere, Category = "Options|UI",
		meta = (AllowPrivateAccess = true))
	TSubclassOf<UUIV_FoodInfoWidget> FoodInfoWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Options",
		meta = (AllowPrivateAccess = true))
	uint16 ColumnValue;
};
