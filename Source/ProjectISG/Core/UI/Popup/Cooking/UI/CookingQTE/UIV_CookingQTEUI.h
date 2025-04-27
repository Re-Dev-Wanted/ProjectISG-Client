#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "UIV_CookingQTEUI.generated.h"


class UUIV_CookingQTEKeyPressWidget;

UCLASS()
class PROJECTISG_API UUIV_CookingQTEUI : public UBaseUIView
{
	GENERATED_BODY()

public:
	GETTER(TObjectPtr<UUIV_CookingQTEKeyPressWidget>, QTEKeyPress)

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUIV_CookingQTEKeyPressWidget> QTEKeyPress;
};
