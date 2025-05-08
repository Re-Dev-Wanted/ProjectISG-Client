#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "UIV_FishingUI.generated.h"

class UImage;
class UInteractiveItemUI;

UCLASS()
class PROJECTISG_API UUIV_FishingUI : public UBaseUIView
{
	GENERATED_BODY()

public:
	GETTER(UInteractiveItemUI*, InteractiveUI)

protected:
	UPROPERTY(meta = (BindWidget))
	UInteractiveItemUI* InteractiveUI;

	UPROPERTY(meta = (BindWidget))
	UImage* CatchMarker;
};
