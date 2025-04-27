#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIModel.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "UIM_CookingQTEKeyPressWidget.generated.h"

enum class ECookingQTEKey : uint8;

UCLASS()
class PROJECTISG_API UUIM_CookingQTEKeyPressWidget : public UBaseUIModel
{
	GENERATED_BODY()

public:
	GETTER_EDITABLE(TArray<ECookingQTEKey>, RemainQTEKeys)
	GETTER_SETTER(uint8, MaxQTELength)
	GETTER_SETTER(uint8, CurrentQTEIndex)

private:
	TArray<ECookingQTEKey> RemainQTEKeys;

	uint8 MaxQTELength;
	uint8 CurrentQTEIndex;
};
