#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "UIV_CurrentQuestWidget.generated.h"

class UTextBlock;

UCLASS()
class PROJECTISG_API UUIV_CurrentQuestWidget : public UBaseUIView
{
	GENERATED_BODY()

public:
	GETTER(TObjectPtr<UWidgetAnimation>, QuestAppearAnimation)
	GETTER(TObjectPtr<UTextBlock>, QuestTitle)
	GETTER(TObjectPtr<UTextBlock>, QuestDescription)

private:
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> QuestAppearAnimation;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> QuestTitle;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> QuestDescription;
};
