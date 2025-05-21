#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "UIV_HUDAlertModalWidget.generated.h"

class UTextBlock;
enum class EAlertType : uint8;

UCLASS()
class PROJECTISG_API UUIV_HUDAlertModalWidget : public UBaseUIView
{
	GENERATED_BODY()

public:
	FORCEINLINE TMap<EAlertType, FSlateColor>& GetColorMap()
	{
		return ColorMap;
	}

	GETTER(TObjectPtr<UTextBlock>, AlertMessage)

private:
	UPROPERTY(EditDefaultsOnly, Category = "Options",
		meta = (AllowPrivateAccess = true))
	TMap<EAlertType, FSlateColor> ColorMap;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> AlertMessage;
};
