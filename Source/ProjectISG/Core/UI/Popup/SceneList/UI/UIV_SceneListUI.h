#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "UIV_SceneListUI.generated.h"

class UImage;
class UBorder;
class UOverlay;

UCLASS()
class PROJECTISG_API UUIV_SceneListUI : public UBaseUIView
{
	GENERATED_BODY()

public:
	GETTER(TObjectPtr<UImage>, SceneImage)
	GETTER(TObjectPtr<UOverlay>, SkipLoadingWidget)
	GETTER(TObjectPtr<UBorder>, CircularLoadingWidget)

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> SkipLoadingWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> CircularLoadingWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> SceneImage;
};
