#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "UIV_DiaryEditUI.generated.h"

class UButton;
class UMultiLineEditableTextBox;
class UTextBlock;
class UImage;

UCLASS()
class PROJECTISG_API UUIV_DiaryEditUI : public UBaseUIView
{
	GENERATED_BODY()

public:
	GETTER(TObjectPtr<UTextBlock>, DiaryDayText)
	GETTER(TObjectPtr<UMultiLineEditableTextBox>, DiaryDescription)
	GETTER(TObjectPtr<UButton>, SaveButton)

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DiaryDayText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMultiLineEditableTextBox> DiaryDescription;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SaveButton;
};
