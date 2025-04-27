#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIModel.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "UIM_CookingQTEKeyPressWidget.generated.h"

class UUIV_CookingQTEKeyWidget;
enum class ECookingQTEKey : uint8;

UCLASS()
class PROJECTISG_API UUIM_CookingQTEKeyPressWidget : public UBaseUIModel
{
	GENERATED_BODY()

public:
	GETTER_EDITABLE(TArray<ECookingQTEKey>, RemainQTEKeys)
	GETTER_EDITABLE(TArray<TObjectPtr<UUIV_CookingQTEKeyWidget>>, KeyWidgets)
	GETTER_SETTER(uint8, CurrentQTEIndex)

private:
	TArray<ECookingQTEKey> RemainQTEKeys;
	TArray<TObjectPtr<UUIV_CookingQTEKeyWidget>> KeyWidgets;

	uint8 CurrentQTEIndex;
};
