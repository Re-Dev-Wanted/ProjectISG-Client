#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "UIC_SceneListUI.generated.h"

class UInputAction;

DECLARE_DELEGATE(FOnSceneListEndNotified);

UCLASS()
class PROJECTISG_API UUIC_SceneListUI : public UBaseUIController
{
	GENERATED_BODY()

public:
	FOnSceneListEndNotified OnSceneListEndNotified;

protected:
	virtual void AppearUI(const EUILayer Layer) override;

	virtual void InitializeController(UBaseUIView* NewView,
	                                  UBaseUIModel* NewModel) override;

	virtual void
	BindInputAction(UEnhancedInputComponent* InputComponent) override;

private:
	FTimerHandle SceneCutChangeTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Options|Input",
		meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> SkipSceneAction;

	UFUNCTION()
	void OnTriggerSkipSceneAction();

	UFUNCTION()
	void OnEndSkipSceneAction();

	void SetSkipCircularPercent(const float Percent);

	UFUNCTION()
	void MoveToNextScene();
};
