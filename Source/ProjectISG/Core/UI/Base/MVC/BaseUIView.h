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
	GETTER(TObjectPtr<UWidgetAnimation>, DefaultTickAnimation)
	GETTER(TObjectPtr<UWidgetAnimation>, DefaultStartAnimation)
	GETTER(TObjectPtr<UWidgetAnimation>, DefaultEndAnimation)
	GETTER_EDITABLE(TObjectPtr<UBaseUIController>, Controller)

	FWidgetAnimationDynamicEvent TickAnimationEndNotified;
	FWidgetAnimationDynamicEvent StartAnimationFinishNotified;
	FWidgetAnimationDynamicEvent EndAnimationFinishNotified;

protected:
	virtual void NativeConstruct() override;
	GETTER_EDITABLE(TObjectPtr<UBaseUIModel>, Model)

private:
	UPROPERTY(Transient, meta=(BindWidgetAnimOptional))
	TObjectPtr<UWidgetAnimation> DefaultTickAnimation;

	UPROPERTY(Transient, meta=(BindWidgetAnimOptional))
	TObjectPtr<UWidgetAnimation> DefaultStartAnimation;

	UPROPERTY(Transient, meta=(BindWidgetAnimOptional))
	TObjectPtr<UWidgetAnimation> DefaultEndAnimation;

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
