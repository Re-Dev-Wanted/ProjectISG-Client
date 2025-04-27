#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "UIV_CookingQTEKeyPressWidget.generated.h"

class UUIV_CookingQTEKeyWidget;
class UHorizontalBox;

UCLASS()
class PROJECTISG_API UUIV_CookingQTEKeyPressWidget : public UBaseUIView
{
	GENERATED_BODY()

public:
	GETTER(TObjectPtr<UHorizontalBox>, KeySlots)
	GETTER(TSubclassOf<UUIV_CookingQTEKeyWidget>, QTEKeyWidgetClass)

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> KeySlots;

	UPROPERTY(EditAnywhere, Category = "Options|UI"
		, meta = (AllowPrivateAccess = true))
	TSubclassOf<UUIV_CookingQTEKeyWidget> QTEKeyWidgetClass;
};
