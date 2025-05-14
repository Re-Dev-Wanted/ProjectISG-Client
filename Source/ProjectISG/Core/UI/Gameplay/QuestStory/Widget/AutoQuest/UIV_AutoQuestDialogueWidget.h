#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "UIV_AutoQuestDialogueWidget.generated.h"

class UMultiLineEditableTextBox;
class UTextBlock;

UCLASS()
class PROJECTISG_API UUIV_AutoQuestDialogueWidget : public UBaseUIView
{
	GENERATED_BODY()

public:
	FWidgetAnimationDynamicEvent OnDialogueEndNotified;

	GETTER(TObjectPtr<UTextBlock>, DialogueOwner)
	GETTER(TObjectPtr<UMultiLineEditableTextBox>, DialogueText)
	GETTER(TObjectPtr<UWidgetAnimation>, DialogueAnimation)

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DialogueOwner;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMultiLineEditableTextBox> DialogueText;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> DialogueAnimation;
};
