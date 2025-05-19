#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "UIV_QuestItemWidget.generated.h"

class UButton;
class UOverlay;
class UTextBlock;

UCLASS()
class PROJECTISG_API UUIV_QuestItemWidget : public UBaseUIView,
                                            public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	GETTER(TObjectPtr<UButton>, QuestItemButton)
	GETTER(TObjectPtr<UTextBlock>, QuestTitle)
	GETTER(TObjectPtr<UTextBlock>, QuestStatus)
	GETTER(TObjectPtr<UTextBlock>, MaxFinishQuestCount)
	GETTER(TObjectPtr<UTextBlock>, CurrentFinishQuestCount)

protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> QuestItemButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> QuestTitle;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> QuestStatus;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentFinishQuestCount;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MaxFinishQuestCount;
};
