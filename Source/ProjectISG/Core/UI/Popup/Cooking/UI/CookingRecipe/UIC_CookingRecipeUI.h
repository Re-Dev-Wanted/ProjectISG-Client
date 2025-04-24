#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "UIC_CookingRecipeUI.generated.h"

class UInputAction;

UCLASS()
class PROJECTISG_API UUIC_CookingRecipeUI : public UBaseUIController
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Options|Input",
		meta = (AllowPrivateAccess = true, EditCondition = "IsInputAccess"))
	TObjectPtr<UInputAction> CloseCookingRecipeUI;

	virtual void
	BindInputAction(UEnhancedInputComponent* InputComponent) override;

	UFUNCTION()
	void OnCloseCookingRecipeUI();
};
