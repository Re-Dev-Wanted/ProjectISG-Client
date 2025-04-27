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
	GETTER(uint8, MinAccessValue)
	GETTER(uint8, MaxAccessValue)
	GETTER_SETTER(TArray<ECookingQTEKey>, RemainQTEKeys)
	GETTER_SETTER(TArray<TObjectPtr<UUIV_CookingQTEKeyWidget>>, KeyWidgets)
	GETTER_SETTER(uint8, CurrentQTEIndex)

private:
	UPROPERTY(EditDefaultsOnly, Category = "Options|Play"
		, meta=(AllowPrivateAccess = true, ClampMin = "1", ClampMax = "10"))
	uint8 MinAccessValue = 1;

	UPROPERTY(EditDefaultsOnly, Category = "Options|Play"
		, meta=(AllowPrivateAccess = true, ClampMin = "1", ClampMax = "10"))
	uint8 MaxAccessValue = 5;

	TArray<ECookingQTEKey> RemainQTEKeys;
	TArray<TObjectPtr<UUIV_CookingQTEKeyWidget>> KeyWidgets;

	uint8 CurrentQTEIndex;
};
