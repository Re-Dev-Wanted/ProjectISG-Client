#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "UIV_QuestShowItemInfoWidget.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class PROJECTISG_API UUIV_QuestShowItemInfoWidget : public UBaseUIView
{
	GENERATED_BODY()

public:
	GETTER(TObjectPtr<UImage>, IngredientThumbnail)
	GETTER(TObjectPtr<UTextBlock>, CurrentIngredientCount)

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> IngredientThumbnail;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentIngredientCount;
};
