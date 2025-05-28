#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/Interfaces/UIHandler.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIModel.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "UIM_LootContainerUI.generated.h"

UCLASS()
class PROJECTISG_API UUIM_LootContainerUI : public UBaseUIModel
{
	GENERATED_BODY()

public:
	GETTER_SETTER(TScriptInterface<IUIHandler>, UIHandler)

private:
	TScriptInterface<IUIHandler> UIHandler = nullptr;
};
