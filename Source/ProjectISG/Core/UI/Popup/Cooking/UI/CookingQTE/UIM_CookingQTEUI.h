#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIModel.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "UIM_CookingQTEUI.generated.h"

enum class ECookingQTEType : uint8;
enum class ECookingQTEStatus : uint8;

UCLASS()
class PROJECTISG_API UUIM_CookingQTEUI : public UBaseUIModel
{
	GENERATED_BODY()

public:
	GETTER_SETTER(ECookingQTEType, CurrentQTEType)
	GETTER_SETTER(uint8, QTEScore)
	GETTER_SETTER(ECookingQTEStatus, QTEStatus)

private:
	ECookingQTEType CurrentQTEType;

	ECookingQTEStatus QTEStatus;

	uint8 QTEScore = 0;
};
