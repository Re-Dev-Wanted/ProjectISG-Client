#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "UI_EscapeButton.generated.h"

class UButton;

UCLASS()
class PROJECTISG_API UUI_EscapeButton : public UUserWidget
{
	GENERATED_BODY()

public:
	GETTER(TObjectPtr<UButton>, Button)

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button;
};
