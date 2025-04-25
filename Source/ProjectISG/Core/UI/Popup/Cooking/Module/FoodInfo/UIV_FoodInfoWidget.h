#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "UIV_FoodInfoWidget.generated.h"

class UImage;

UCLASS()
class PROJECTISG_API UUIV_FoodInfoWidget : public UBaseUIView
{
	GENERATED_BODY()

public:
	GETTER(TObjectPtr<UImage>, FoodThumbnail)

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> FoodThumbnail;
};
