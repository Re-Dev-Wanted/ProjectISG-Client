#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "UIC_WorkbenchUI.generated.h"

class UInputAction;

UCLASS()
class PROJECTISG_API UUIC_WorkbenchUI : public UBaseUIController
{
	GENERATED_BODY()
	
protected:
	virtual void BindInputAction(UEnhancedInputComponent* InputComponent) override;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> CloseAction;
};
