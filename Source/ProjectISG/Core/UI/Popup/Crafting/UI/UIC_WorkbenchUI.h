#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "UIC_WorkbenchUI.generated.h"

class IUIHandler;
class UInputAction;

DECLARE_DELEGATE_OneParam(FOnSelectedCraftingRecipe, uint16);

UCLASS()
class PROJECTISG_API UUIC_WorkbenchUI : public UBaseUIController
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void OnUpdateSelectedRecipeUI(uint16 RecipeId);
	
	SETTER(TScriptInterface<IUIHandler>, UIHandler)

	FOnSelectedCraftingRecipe OnSelectedRecipe;
	
protected:
	virtual void AppearUI() override;
	
	virtual void BindInputAction(UEnhancedInputComponent* InputComponent) override;
	
	UFUNCTION()
	void StartCrafting();

	UFUNCTION()
	void CloseUI();

	void Logging(uint16 ItemId);
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> CloseAction;

	TScriptInterface<IUIHandler> UIHandler;

private:
	bool IsInitialized = false;
};
