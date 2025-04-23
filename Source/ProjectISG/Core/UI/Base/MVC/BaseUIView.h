#pragma once

#include "CoreMinimal.h"
#include "BaseUIController.h"
#include "Blueprint/UserWidget.h"
#include "BaseUIView.generated.h"

class UBaseUIController;
class UBaseUIModel;

UCLASS()
class PROJECTISG_API UBaseUIView : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void InitializeUI(UBaseUIModel* BaseUIModel,
	                          UBaseUIController* BaseUIController);

protected:
	UPROPERTY()
	TObjectPtr<UBaseUIModel> Model;

	UPROPERTY()
	TObjectPtr<UBaseUIController> Controller;
};
