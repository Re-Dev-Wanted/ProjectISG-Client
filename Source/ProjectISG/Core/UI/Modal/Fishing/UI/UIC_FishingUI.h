#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "UIC_FishingUI.generated.h"

class UInputAction;

UCLASS()
class PROJECTISG_API UUIC_FishingUI : public UBaseUIController
{
	GENERATED_BODY()

public:
	void SetUI(bool bIsActive, const FString& Key, const FString& Text);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Options|Input",
		meta = (AllowPrivateAccess = true, EditCondition = "IsInputAccess"))
	TObjectPtr<UInputAction> CatchAction;

	virtual void BindInputAction(UEnhancedInputComponent* InputComponent) override;

	UFUNCTION()
	void OnAction();
};
