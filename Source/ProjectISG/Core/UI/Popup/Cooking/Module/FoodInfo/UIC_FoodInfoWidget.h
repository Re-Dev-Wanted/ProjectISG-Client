#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "UIC_FoodInfoWidget.generated.h"

UCLASS()
class PROJECTISG_API UUIC_FoodInfoWidget : public UBaseUIController
{
	GENERATED_BODY()

public:
	void SetFoodInfo(const uint32 RecipeId);

protected:
	virtual void InitializeController(UBaseUIView* NewView
									, UBaseUIModel* NewModel) override;

private:
	UFUNCTION()
	void OnSelectFoodData();
};
