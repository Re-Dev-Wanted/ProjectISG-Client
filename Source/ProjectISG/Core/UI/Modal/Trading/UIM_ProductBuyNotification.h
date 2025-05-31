#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIModel.h"
#include "ProjectISG/Core/UI/Popup/Trading/UI/UIM_TradingUI.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "UIM_ProductBuyNotification.generated.h"

UCLASS()
class PROJECTISG_API UUIM_ProductBuyNotification : public UBaseUIModel
{
	GENERATED_BODY()

public:
	GETTER_SETTER(uint32, ClickedProductId);
	GETTER_SETTER(ETradingState, TradingState)
	GETTER_SETTER(uint32, Count)

private:
	uint32 ClickedProductId;

	ETradingState TradingState;

	uint32 Count = 1;
};
