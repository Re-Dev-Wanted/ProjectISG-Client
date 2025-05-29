#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "UIV_MbtiAskUI.generated.h"

class UEditableText;
class UEditableTextBox;
class UMultiLineEditableTextBox;
class UButton;

UCLASS()
class PROJECTISG_API UUIV_MbtiAskUI : public UBaseUIView
{
	GENERATED_BODY()

public:
	GETTER(TObjectPtr<UMultiLineEditableTextBox>, AskTextBox)
	GETTER(TObjectPtr<UEditableText>, AnswerTextBox)
	GETTER(TObjectPtr<UButton>, SaveButton)
	GETTER(TObjectPtr<UWidgetAnimation>, LoadingAnimation)

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMultiLineEditableTextBox> AskTextBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableText> AnswerTextBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SaveButton;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> LoadingAnimation;
};
