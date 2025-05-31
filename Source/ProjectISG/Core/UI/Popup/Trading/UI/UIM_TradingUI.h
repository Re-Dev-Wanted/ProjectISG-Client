#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/UIEnum.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIModel.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "UIM_TradingUI.generated.h"

UCLASS()
class PROJECTISG_API UUIM_TradingUI : public UBaseUIModel
{
	GENERATED_BODY()

public:
	GETTER(ETradingState, CurrentState)
	GETTER_SETTER(uint16, SelectedId)

	void SetCurrentState(ETradingState State)
	{
		CurrentState = State;
		SelectedId = 0;
	}

private:
	UPROPERTY()
	ETradingState CurrentState;

	UPROPERTY()
	uint16 SelectedId;
};
