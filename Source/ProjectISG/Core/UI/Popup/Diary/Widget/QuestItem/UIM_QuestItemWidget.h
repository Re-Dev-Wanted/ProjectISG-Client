#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIModel.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "UIM_QuestItemWidget.generated.h"

UCLASS()
class PROJECTISG_API UUIM_QuestItemWidget : public UBaseUIModel
{
	GENERATED_BODY()

public:
	GETTER_SETTER(FString, CurrentQuestId)

private:
	FString CurrentQuestId;
};
