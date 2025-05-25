#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "UIC_QuestItemWidget.generated.h"

struct FEventReply;

UCLASS()
class PROJECTISG_API UUIC_QuestItemWidget : public UBaseUIController
{
	GENERATED_BODY()

public:
	void InitializeData(const FString& QuestId);

private:
	UFUNCTION()
	void OnClickQuestItemWidget();

	void SetQuestDefaultInformation(const FString& QuestId);
	void SetQuestRewardList(const FString& QuestId) const;
	void SetQuestStatus(const FString& QuestId) const;
};
