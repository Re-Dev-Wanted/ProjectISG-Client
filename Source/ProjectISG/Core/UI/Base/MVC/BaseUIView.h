#pragma once

#include "Blueprint/UserWidget.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "BaseUIView.generated.h"

class UBaseUIModel;
class UBaseUIController;

UCLASS(Abstract, Blueprintable)
class PROJECTISG_API UBaseUIView : public UUserWidget
{
	GENERATED_BODY()

public:
	GETTER_EDITABLE(TObjectPtr<UBaseUIController>, Controller)

protected:
	virtual void NativeConstruct() override;
	GETTER_EDITABLE(TObjectPtr<UBaseUIModel>, Model)

private:
	UPROPERTY(EditDefaultsOnly, Category = "Options",
		meta=(AllowPrivateAccess = true))
	TSubclassOf<UBaseUIModel> ModelClass;

	UPROPERTY(EditDefaultsOnly, Category = "Options",
		meta=(AllowPrivateAccess = true))
	TSubclassOf<UBaseUIController> ControllerClass;

	UPROPERTY()
	TObjectPtr<UBaseUIModel> Model;

	UPROPERTY()
	TObjectPtr<UBaseUIController> Controller;
};
