#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "UIC_MediaSceneListUI.generated.h"

DECLARE_DYNAMIC_DELEGATE(FOnEndMediaSceneNotified);

class UInputAction;

UCLASS()
class PROJECTISG_API UUIC_MediaSceneListUI : public UBaseUIController
{
	GENERATED_BODY()

public:
	FOnEndMediaSceneNotified OnEndMediaSceneNotified;

protected:
	virtual void AppearUI() override;

	virtual void
	BindInputAction(UEnhancedInputComponent* InputComponent) override;

private:
	UFUNCTION()
	void OnEndMediaScene();

	UPROPERTY(EditDefaultsOnly, Category = "Options|Input",
		meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> SkipSceneAction;

	void SetSkipCircularPercent(const float Percent);

	UFUNCTION()
	void OnTriggerSkipSceneAction();

	UFUNCTION()
	void OnEndSkipSceneAction();
};
