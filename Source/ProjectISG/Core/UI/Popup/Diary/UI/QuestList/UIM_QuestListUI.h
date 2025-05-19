#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIModel.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "UIM_QuestListUI.generated.h"

UCLASS()
class PROJECTISG_API UUIM_QuestListUI : public UBaseUIModel
{
	GENERATED_BODY()

public:
	GETTER_SETTER(FString, CurrentSelectQuestId)

private:
	FString CurrentSelectQuestId;
};
