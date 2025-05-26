// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "UIC_ExitInteractUI.generated.h"

class UInputAction;

UCLASS()
class PROJECTISG_API UUIC_ExitInteractUI : public UBaseUIController
{
	GENERATED_BODY()

public:
	virtual void AppearUI() override;
	
	void SetUI(const FString& Key, const FString& Text);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Options|Input",
		meta = (AllowPrivateAccess = true, EditCondition = "IsInputAccess"))
	TObjectPtr<UInputAction> ExitInteractAction;

	UPROPERTY(EditDefaultsOnly, Category = "Options|Input",
		meta = (AllowPrivateAccess = true, EditCondition = "IsInputAccess"))
	TObjectPtr<UInputAction> RotateAction;

	virtual void BindInputAction(UEnhancedInputComponent* InputComponent) override;

	UFUNCTION()
	void ExitInteract();

	UFUNCTION()
	void Look(const FInputActionValue& Value);
	
};
