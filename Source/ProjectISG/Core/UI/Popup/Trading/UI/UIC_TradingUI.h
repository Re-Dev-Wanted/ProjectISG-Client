#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "UIC_TradingUI.generated.h"

UCLASS()
class PROJECTISG_API UUIC_TradingUI : public UBaseUIController
{
	GENERATED_BODY()

protected:
	virtual void
	BindInputAction(UEnhancedInputComponent* InputComponent) override;
	
	virtual void AppearUI() override;

	UFUNCTION()
	void ChangeBuyState();

	UFUNCTION()
	void ChangeSellState();

	UFUNCTION()
	void OnCloseTradingUI();

	UFUNCTION()
	void OnTrade();

	void RefreshList();
public:
	void OnUpdateSelectedProduct(uint16 ProductId);
	
	void UpdateGoldText();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Options|Input"
		, meta = (AllowPrivateAccess = true, EditCondition = "IsInputAccess"))
	class UInputAction* CloseTradingUI;
};
