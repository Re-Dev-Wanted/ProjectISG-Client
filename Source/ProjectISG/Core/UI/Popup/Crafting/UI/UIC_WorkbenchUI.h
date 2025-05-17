#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "UIC_WorkbenchUI.generated.h"

class IUIHandler;
class UInputAction;

UCLASS()
class PROJECTISG_API UUIC_WorkbenchUI : public UBaseUIController
{
	GENERATED_BODY()

public:
	SETTER(TScriptInterface<IUIHandler>, UIHandler)
	
protected:
	virtual void BindInputAction(UEnhancedInputComponent* InputComponent) override;
	
	UFUNCTION()
	void StartCrafting();

	UFUNCTION()
	void CloseUI();
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> CloseAction;

	TScriptInterface<IUIHandler> UIHandler;
};
