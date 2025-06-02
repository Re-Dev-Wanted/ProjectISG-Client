#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestCompleteNotification.generated.h"

class UUMGSequencePlayer;
class UWidgetAnimation;

UCLASS()
class PROJECTISG_API UQuestCompleteNotification : public UUserWidget
{
	GENERATED_BODY()

public:
	void OnNotify();

protected:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> AppearAnimation;

	UPROPERTY()
	TObjectPtr<UUMGSequencePlayer> AnimationPlayer;
};
