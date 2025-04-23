#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "UObject/Object.h"
#include "BaseUIController.generated.h"

class UBaseUIView;
class UBaseUIModel;

UCLASS(Blueprintable)
class PROJECTISG_API UBaseUIController : public UObject
{
	GENERATED_BODY()

public:
	virtual void InitializeController(UBaseUIView* NewView,
	                                  UBaseUIModel* NewModel);

	GETTER(TObjectPtr<UBaseUIView>, View)

protected:
	GETTER_EDITABLE(TObjectPtr<UBaseUIModel>, Model)

private:
	UPROPERTY()
	TObjectPtr<UBaseUIModel> Model;

	UPROPERTY()
	TObjectPtr<UBaseUIView> View;
};
