#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "UIV_QuestListUI.generated.h"

class UVerticalBox;
class UUIV_QuestRequiredTextWidget;
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
	GETTER(TObjectPtr<UMultiLineEditableTextBox>, QuestScenario)
	GETTER(TObjectPtr<UVerticalBox>, QuestRequireList)
	GETTER(TObjectPtr<UTextBlock>, CurrentDateTime)
	GETTER(TObjectPtr<UListView>, QuestListView)
	GETTER(TObjectPtr<UTextBlock>, QuestChapter)
	GETTER(TObjectPtr<UTextBlock>, QuestTitle)
	GETTER(TObjectPtr<UTextBlock>, QuestHint)
	GETTER(TObjectPtr<UOverlay>, QuestInfo)
	GETTER(TObjectPtr<UTextBlock>, UserId)

	GETTER(FSlateColor, RequiredQuestHasDoneColor)
	GETTER(FSlateColor, RequiredQuestDefaultColor)

	GETTER(TSubclassOf<UUIV_QuestRequiredTextWidget>, QuestRequiredTextClass)

private:
#pragma region Options
	UPROPERTY(EditAnywhere, Category = "Options",
		meta = (AllowPrivateAccess = true))
	TSubclassOf<UUIV_QuestRequiredTextWidget> QuestRequiredTextClass;
#pragma endregion

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentDateTime;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> UserId;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> QuestChapter;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> QuestTitle;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> QuestHint;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMultiLineEditableTextBox> QuestScenario;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UListView> QuestListView;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> QuestInfo;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> QuestRequireList;

#pragma region ColorOption
	UPROPERTY(EditDefaultsOnly, Category = "Options|Color",
		meta = (AllowPrivateAccess = true))
	FSlateColor RequiredQuestHasDoneColor;

	UPROPERTY(EditDefaultsOnly, Category = "Options|Color",
		meta = (AllowPrivateAccess = true))
	FSlateColor RequiredQuestDefaultColor;
#pragma endregion
};
