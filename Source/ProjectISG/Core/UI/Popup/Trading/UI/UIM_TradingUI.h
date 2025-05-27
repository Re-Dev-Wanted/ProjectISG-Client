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
	GETTER_SETTER(uint32, ClickedInventoryItem)
	GETTER_SETTER(uint16, ClickedInventorySlotIndex)

private:
	UPROPERTY()
	ETradingState CurrentState;
	
	UPROPERTY()
	uint32 ClickedInventoryItem;

	UPROPERTY()
	uint16 ClickedInventorySlotIndex;
};
