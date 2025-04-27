#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "UIV_CookingQTEKeyWidget.generated.h"

class UBorder;
class UTextBlock;

UCLASS()
class PROJECTISG_API UUIV_CookingQTEKeyWidget : public UBaseUIView
{
	GENERATED_BODY()

public:
	GETTER(TObjectPtr<UTextBlock>, PressKey)
	GETTER(TObjectPtr<UBorder>, IsDoneBorder)
	GETTER(TObjectPtr<UBorder>, IsMissBorder)

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> PressKey;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> IsDoneBorder;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> IsMissBorder;
};
