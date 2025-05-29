#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "UIV_MbtiAskUI.generated.h"

class UTextBlock;
class UMultiLineEditableText;
class UProgressBar;
class UMultiLineEditableTextBox;
class UButton;

UCLASS()
class PROJECTISG_API UUIV_MbtiAskUI : public UBaseUIView
{
	GENERATED_BODY()

public:
	GETTER(TObjectPtr<UMultiLineEditableTextBox>, AskTextBox)
	GETTER(TObjectPtr<UMultiLineEditableText>, AnswerTextArea)
	GETTER(TObjectPtr<UButton>, SaveButton)
	GETTER(TObjectPtr<UTextBlock>, CurrentFlowCount)

protected:
	virtual void
	NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMultiLineEditableTextBox> AskTextBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMultiLineEditableText> AnswerTextArea;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SaveButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> FlowProgressBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentFlowCount;
};
