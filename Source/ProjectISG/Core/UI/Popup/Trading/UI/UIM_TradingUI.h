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
	GETTER_SETTER(uint32, ClickedInventoryItem);

private:
	UPROPERTY()
	uint32 ClickedInventoryItem;
};
