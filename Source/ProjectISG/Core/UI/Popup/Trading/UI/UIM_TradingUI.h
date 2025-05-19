// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIModel.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "UIM_TradingUI.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTISG_API UUIM_TradingUI : public UBaseUIModel
{
	GENERATED_BODY()

public:
	GETTER_SETTER(uint32, ClickedInventoryItem)
	GETTER_SETTER(uint16, ClickedInventorySlotIndex)

private:
	UPROPERTY()
	uint32 ClickedInventoryItem;

	UPROPERTY()
	uint16 ClickedInventorySlotIndex;
};
