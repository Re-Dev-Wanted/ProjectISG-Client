#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "UIC_CookingRecipeUI.generated.h"

class UInputAction;

UCLASS()
class PROJECTISG_API UUIC_CookingRecipeUI : public UBaseUIController
{
	GENERATED_BODY()

public:
	void SetSelectedCookingRecipe(const uint32 RecipeId);

	void TurnOffSelectedCookingRecipe();

protected:
	virtual void AppearUI() override;

	virtual void
	BindInputAction(UEnhancedInputComponent* InputComponent) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Options|Input"
		, meta = (AllowPrivateAccess = true, EditCondition = "IsInputAccess"))
	TObjectPtr<UInputAction> CloseCookingRecipeUI;

	bool CanCookByRecipeId(const uint32 RecipeId);

	UFUNCTION()
	void OnCloseCookingRecipeUI();

	UFUNCTION()
	void StartCooking();
};
