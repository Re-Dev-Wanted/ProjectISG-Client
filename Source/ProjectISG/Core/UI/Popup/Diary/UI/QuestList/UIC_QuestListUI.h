#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "UIC_QuestListUI.generated.h"

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
};
