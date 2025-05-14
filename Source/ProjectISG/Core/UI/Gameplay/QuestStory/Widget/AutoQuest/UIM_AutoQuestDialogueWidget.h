#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIModel.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "UIM_AutoQuestDialogueWidget.generated.h"

UCLASS()
class PROJECTISG_API UUIM_AutoQuestDialogueWidget : public UBaseUIModel
{
	GENERATED_BODY()

public:
	GETTER_SETTER(uint8, CurrentQuestDialogueIndex)

private:
	uint8 CurrentQuestDialogueIndex = 0;
};
