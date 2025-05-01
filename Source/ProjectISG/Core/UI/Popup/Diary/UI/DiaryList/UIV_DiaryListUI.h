#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "UIV_DiaryListUI.generated.h"

class UButton;
class UMultiLineEditableTextBox;
class UTextBlock;
class UImage;

UCLASS()
class PROJECTISG_API UUIV_DiaryListUI : public UBaseUIView
{
	GENERATED_BODY()

public:
	GETTER(TObjectPtr<UTextBlock>, DiaryDayText)
	GETTER(TObjectPtr<UMultiLineEditableTextBox>, DiaryDescription)
	GETTER(TObjectPtr<UButton>, PrevButton)
	GETTER(TObjectPtr<UButton>, NextButton)

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DiaryDayText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMultiLineEditableTextBox> DiaryDescription;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> PrevButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> NextButton;
};
