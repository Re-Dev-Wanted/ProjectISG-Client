#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "UIV_FoodInfoWidget.generated.h"

class UTextBlock;
class UImage;

UCLASS()
class PROJECTISG_API UUIV_FoodInfoWidget : public UBaseUIView
{
	GENERATED_BODY()

public:
	GETTER(TObjectPtr<UImage>, FoodThumbnail)
	GETTER(TObjectPtr<UTextBlock>, FoodName)

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> FoodThumbnail;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> FoodName;
};
