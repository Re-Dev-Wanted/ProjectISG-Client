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
	virtual void InitializeController(UBaseUIView* NewView,
	                                  UBaseUIModel* NewModel);

	virtual void InitializeSettingToPlayerController(APlayerController* PC);

	virtual void AppearUI(const EUILayer Layer);
	virtual void DisappearUI();

	virtual void PopUIFromPlayerController();

	GETTER(TObjectPtr<UBaseUIView>, View)

protected:
	GETTER_EDITABLE(TObjectPtr<UBaseUIModel>, Model)

	virtual void BindInputAction(UEnhancedInputComponent* InputComponent);

	void ClearInputMappingContext();

private:
	UPROPERTY()
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(EditDefaultsOnly, Category = "Options",
		meta= (AllowPrivateAccess = true))
	bool IsInputAccess;

	UPROPERTY(EditDefaultsOnly, Category = "Options|Input",
		meta = (AllowPrivateAccess = true, EditCondition = "IsInputAccess"))
	TObjectPtr<UInputMappingContext> UIMappingContext;

	UPROPERTY()
	TObjectPtr<UBaseUIModel> Model;

	UPROPERTY()
	TObjectPtr<UBaseUIView> View;

	void ChangeInputActionToUI();
};
