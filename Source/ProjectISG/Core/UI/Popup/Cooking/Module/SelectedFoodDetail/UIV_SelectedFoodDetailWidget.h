#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "UIV_SelectedFoodDetailWidget.generated.h"

class UUIV_FoodIngredientInfoWidget;
class UScrollBox;
class UTextBlock;

UCLASS()
class PROJECTISG_API UUIV_SelectedFoodDetailWidget : public UBaseUIView
{
	GENERATED_BODY()

public:
	GETTER(TObjectPtr<UTextBlock>, SelectedFoodName)
	GETTER(TObjectPtr<UScrollBox>, IngredientListScroll)
	GETTER(TSubclassOf<UUIV_FoodIngredientInfoWidget>
			, FoodIngredientInfoWidgetClass)

protected:
	virtual void NativePreConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SelectedFoodName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> IngredientListScroll;

	UPROPERTY(EditAnywhere, Category = "Options|UI"
		, meta = (AllowPrivateAccess = true))
	TSubclassOf<UUIV_FoodIngredientInfoWidget> FoodIngredientInfoWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Options|UI"
		, meta = (AllowPrivateAccess = true))
	uint16 DebugFoodIngredientInfoCount = 10;
};
