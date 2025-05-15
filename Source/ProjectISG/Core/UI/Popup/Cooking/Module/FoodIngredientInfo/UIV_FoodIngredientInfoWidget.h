#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "UIV_FoodIngredientInfoWidget.generated.h"

class UTextBlock;
class UImage;

UCLASS()
class PROJECTISG_API UUIV_FoodIngredientInfoWidget : public UBaseUIView
{
	GENERATED_BODY()

public:
	GETTER(TObjectPtr<UImage>, IngredientThumbnail)
	GETTER(TObjectPtr<UTextBlock>, IngredientCount)
	GETTER(TObjectPtr<UTextBlock>, CurrentIngredientCount)

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> IngredientThumbnail;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> IngredientCount;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentIngredientCount;
};
