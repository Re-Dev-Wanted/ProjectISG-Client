// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "UIC_TradingUI.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTISG_API UUIC_TradingUI : public UBaseUIController
{
	GENERATED_BODY()

	virtual void
	BindInputAction(UEnhancedInputComponent* InputComponent) override;

	UFUNCTION()
	void OnCloseTradingUI();
	UPROPERTY(EditDefaultsOnly, Category = "Options|Input"
		, meta = (AllowPrivateAccess = true, EditCondition = "IsInputAccess"))
	class UInputAction* CloseTradingUI;

public:
	void UpdateGoldText();

	void UpdateInventory();

	UFUNCTION()
	void DetectDragItem(uint16 ItemId, uint16 SlotIndex);
};
