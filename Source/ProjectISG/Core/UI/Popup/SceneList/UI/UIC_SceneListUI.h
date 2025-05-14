#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "UIC_SceneListUI.generated.h"

class UInputAction;

UCLASS()
class PROJECTISG_API UUIC_SceneListUI : public UBaseUIController
{
	GENERATED_BODY()

protected:
	virtual void InitializeController(UBaseUIView* NewView,
	                                  UBaseUIModel* NewModel) override;

	virtual void
	BindInputAction(UEnhancedInputComponent* InputComponent) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Options|Input",
		meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> SkipSceneAction;

	UFUNCTION()
	void OnTriggerSkipSceneAction();

	UFUNCTION()
	void OnEndSkipSceneAction();

	void SetSkipCircularPercent(const float Percent);
};
