#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "UIV_CookingQTEKeyPressWidget.generated.h"

class UProgressBar;
class UUIV_CookingQTEKeyWidget;
class UHorizontalBox;

UCLASS()
class PROJECTISG_API UUIV_CookingQTEKeyPressWidget : public UBaseUIView
{
	GENERATED_BODY()

public:
	GETTER(TObjectPtr<UHorizontalBox>, KeySlots)
	GETTER(TObjectPtr<UProgressBar>, RemainTimeBar)
	GETTER(TSubclassOf<UUIV_CookingQTEKeyWidget>, QTEKeyWidgetClass)

protected:
	virtual void
	NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> KeySlots;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> RemainTimeBar;

	UPROPERTY(EditAnywhere, Category = "Options|UI"
		, meta = (AllowPrivateAccess = true))
	TSubclassOf<UUIV_CookingQTEKeyWidget> QTEKeyWidgetClass;
};
