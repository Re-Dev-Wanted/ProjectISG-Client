#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "UIC_QuestListUI.generated.h"

struct FQuestRequireData;

UCLASS()
class PROJECTISG_API UUIC_QuestListUI : public UBaseUIController
{
	GENERATED_BODY()

public:
	void SetQuestInfo(const FString& QuestId);

protected:
	virtual void AppearUI() override;

private:
	void InitializeQuestList();

	void SetQuestRequireData(const FString& QuestId);
	
	void SetQuestRewardData(const FString& QuestId);

	void SetQuestRequireItemData(FQuestRequireData& RequireQuest) const;
};
