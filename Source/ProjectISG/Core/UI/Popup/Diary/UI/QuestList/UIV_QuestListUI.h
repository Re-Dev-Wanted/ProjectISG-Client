#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "UIV_QuestListUI.generated.h"

class UMultiLineEditableTextBox;
class UMultiLineEditableText;
class UOverlay;
class UListView;
class UTextBlock;

UCLASS()
class PROJECTISG_API UUIV_QuestListUI : public UBaseUIView
{
	GENERATED_BODY()

public:
	GETTER(TObjectPtr<UTextBlock>, CurrentDateTime)
	GETTER(TObjectPtr<UListView>, QuestListView)
	GETTER(TObjectPtr<UTextBlock>, QuestChapter)
	GETTER(TObjectPtr<UMultiLineEditableTextBox>, QuestScenario)
	GETTER(TObjectPtr<UTextBlock>, QuestTitle)
	GETTER(TObjectPtr<UOverlay>, QuestInfo)
	GETTER(TObjectPtr<UTextBlock>, UserId)

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentDateTime;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> UserId;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> QuestChapter;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> QuestTitle;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMultiLineEditableTextBox> QuestScenario;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UListView> QuestListView;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> QuestInfo;
};
