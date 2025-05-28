#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "UObject/Object.h"
#include "BaseUIController.generated.h"

enum class EUILayer : uint8;
class UInputMappingContext;
class UBaseUIView;
class UBaseUIModel;

UCLASS(Blueprintable)
class PROJECTISG_API UBaseUIController : public UObject
{
	GENERATED_BODY()

public:
	void StartShowUI(const EUILayer Layer);
	void EndShowUI(const bool IsWidget = false);

	virtual void InitializeController(UBaseUIView* NewView
	                                  , UBaseUIModel* NewModel);

	virtual void InitializeSettingToPlayerController(APlayerController* PC);

	UFUNCTION()
	void PopUIFromPlayerController();

	UFUNCTION()
	void ResetUIFromPlayerController();

	GETTER(TObjectPtr<UBaseUIView>, View)
	GETTER_EDITABLE(TObjectPtr<UBaseUIModel>, Model)
	GETTER(TObjectPtr<APlayerController>, PlayerController)

	GETTER(EUILayer, CurrentLayer)

protected:
	virtual void BindInputAction(UEnhancedInputComponent* InputComponent);

	void ClearInputMappingContext();

	UFUNCTION()
	virtual void AppearUI();

	UFUNCTION()
	virtual void DisappearUI();

private:
	// 기본적으로 UI가 만들어질 때 다 노출처리가 됨으로 기본 true로 설정하고 진행한다.
	bool IsVisibleUI = true;

	EUILayer CurrentLayer;

	UPROPERTY()
	TObjectPtr<APlayerController> PlayerController;

	// 컨트롤러 내부 옵션이기 때문에 Model이 아닌 Controller에 둔다.
	UPROPERTY(EditDefaultsOnly, Category = "Options"
		, meta= (AllowPrivateAccess = true))
	bool IsInputAccess;

	UPROPERTY(EditDefaultsOnly, Category = "Options|Input"
		, meta = (AllowPrivateAccess = true, EditCondition = "IsInputAccess"))
	TObjectPtr<UInputMappingContext> UIMappingContext;

	UPROPERTY()
	TObjectPtr<UBaseUIModel> Model;

	UPROPERTY()
	TObjectPtr<UBaseUIView> View;

	void ChangeInputActionToUI(const bool IsBindAction);

	UFUNCTION()
	void OnFinishDefaultTickAnimation();
};
