#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIModel.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "UIM_TradingUI.generated.h"

UENUM()
enum class ETradingState : uint8
{
	BUY,
	SELL
};

UCLASS()
class PROJECTISG_API UUIM_TradingUI : public UBaseUIModel
{
	GENERATED_BODY()

public:
	GETTER_SETTER(ETradingState, CurrentState)

private:
	UPROPERTY()
	ETradingState CurrentState;
};
