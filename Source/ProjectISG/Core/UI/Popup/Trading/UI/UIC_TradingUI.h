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

protected:
	virtual void
	BindInputAction(UEnhancedInputComponent* InputComponent) override;

	virtual void AppearUI() override;

	UFUNCTION()
	void OnCloseTradingUI();

	UFUNCTION()
	void DetectDragItem(uint16 ItemId, uint16 SlotIndex);

public:
	void UpdateGoldText();

	void UpdateInventory();

	void ClearItemInfoData();

	void SetItemInfoData(const uint8 InventoryIndex);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Options|Input"
		, meta = (AllowPrivateAccess = true, EditCondition = "IsInputAccess"))
	class UInputAction* CloseTradingUI;
};
