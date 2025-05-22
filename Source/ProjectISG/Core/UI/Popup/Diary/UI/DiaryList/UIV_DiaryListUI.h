#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "UIV_DiaryListUI.generated.h"

class UUI_HttpImage;
class UButton;
class UMultiLineEditableTextBox;
class UTextBlock;

UCLASS()
class PROJECTISG_API UUIV_DiaryListUI : public UBaseUIView
{
	GENERATED_BODY()

public:
	GETTER(TObjectPtr<UTextBlock>, DiaryDayText)
	GETTER(TObjectPtr<UTextBlock>, UserId)
	GETTER(TObjectPtr<UMultiLineEditableTextBox>, DiaryDescription)
	GETTER(TObjectPtr<UButton>, PrevButton)
	GETTER(TObjectPtr<UButton>, NextButton)
	GETTER(TObjectPtr<UButton>, ExitButton)
	GETTER(TObjectPtr<UUI_HttpImage>, DiaryImage)

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DiaryDayText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> UserId;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMultiLineEditableTextBox> DiaryDescription;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> PrevButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> NextButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ExitButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUI_HttpImage> DiaryImage;
};
