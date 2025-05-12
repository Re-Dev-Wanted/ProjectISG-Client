#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "UIC_DiaryListUI.generated.h"

class UInputAction;

UCLASS()
class PROJECTISG_API UUIC_DiaryListUI : public UBaseUIController
{
	GENERATED_BODY()

public:
	void InitializeData();

	UFUNCTION()
	void MoveToPrevPage();

	UFUNCTION()
	void MoveToNextPage();

protected:
	virtual void InitializeController(UBaseUIView* NewView
	                                  , UBaseUIModel* NewModel) override;

	virtual void
	BindInputAction(UEnhancedInputComponent* InputComponent) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Options|Input"
		, meta = (AllowPrivateAccess = true, EditCondition = "IsInputAccess"))
	TObjectPtr<UInputAction> CloseDiaryListUI;

	void UpdateDiaryPerPage(const int Page);

	UFUNCTION()
	void OnCloseDiaryListUI();
};
