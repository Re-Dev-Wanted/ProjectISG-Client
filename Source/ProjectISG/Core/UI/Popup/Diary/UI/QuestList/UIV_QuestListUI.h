#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "UIV_QuestListUI.generated.h"

class UTextBlock;

UCLASS()
class PROJECTISG_API UUIV_QuestListUI : public UBaseUIView
{
	GENERATED_BODY()

public:
	GETTER(TObjectPtr<UTextBlock>, CurrentDateTime)
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
};
