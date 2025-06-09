#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "UIV_QuestItemWidget.generated.h"

class UImage;
class UScrollBox;
class UBorder;
class UButton;
class UOverlay;
class UTextBlock;
class UUIV_QuestShowItemInfoWidget;

UCLASS()
class PROJECTISG_API UUIV_QuestItemWidget
	: public UBaseUIView, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	GETTER(TObjectPtr<UButton>, QuestItemButton)
	GETTER(TObjectPtr<UTextBlock>, QuestTitle)
	GETTER(TObjectPtr<UImage>, QuestBorder)
	GETTER(TObjectPtr<UTextBlock>, QuestStatus)
	GETTER(TObjectPtr<UBorder>, QuestPlayingNotify)
	GETTER(TObjectPtr<UTextBlock>, MaxFinishQuestCount)
	GETTER(TObjectPtr<UTextBlock>, CurrentFinishQuestCount)
	GETTER(TObjectPtr<UScrollBox>, RewardPreviewList)
	GETTER(TSubclassOf<UUIV_QuestShowItemInfoWidget>, QuestRewardItemClass)

	GETTER_REF(FLinearColor, DefaultColor)
	GETTER_REF(FLinearColor, SelectedColor)
	GETTER(float, SelectedScale)

protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Options",
		meta = (AllowPrivateAccess = true))
	FLinearColor DefaultColor;

	UPROPERTY(EditDefaultsOnly, Category = "Options",
		meta = (AllowPrivateAccess = true))
	FLinearColor SelectedColor;

	UPROPERTY(EditDefaultsOnly, Category = "Options",
		meta = (AllowPrivateAccess = true))
	float SelectedScale;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> QuestItemButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> QuestBorder;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> QuestTitle;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentFinishQuestCount;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MaxFinishQuestCount;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> QuestStatus;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> QuestPlayingNotify;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> RewardPreviewList;

	UPROPERTY(EditAnywhere, Category = "Options"
		, meta = (AllowPrivateAccess = true))
	TSubclassOf<UUIV_QuestShowItemInfoWidget> QuestRewardItemClass;
};
