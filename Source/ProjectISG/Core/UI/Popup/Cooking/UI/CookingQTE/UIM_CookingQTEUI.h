#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Contents/Cooking/CookingEnum.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIModel.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "UIM_CookingQTEUI.generated.h"

enum class ECookingQTEType : uint8;

UCLASS()
class PROJECTISG_API UUIM_CookingQTEUI : public UBaseUIModel
{
	GENERATED_BODY()

public:
	GETTER_SETTER(ECookingQTEType, CurrentQTEType)

private:
	ECookingQTEType CurrentQTEType = ECookingQTEType::None;
};
