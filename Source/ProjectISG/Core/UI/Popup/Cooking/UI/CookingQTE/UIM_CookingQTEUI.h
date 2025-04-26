#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIModel.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "UIM_CookingQTEUI.generated.h"

UCLASS()
class PROJECTISG_API UUIM_CookingQTEUI : public UBaseUIModel
{
	GENERATED_BODY()

public:
	GETTER_SETTER(bool, IsStartQTE)

private:
	bool IsStartQTE = false;
};
