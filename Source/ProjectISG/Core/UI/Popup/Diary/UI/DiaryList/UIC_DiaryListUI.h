#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "UIC_DiaryListUI.generated.h"

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

private:
	void UpdateDiaryPerPage(const int Page);
};
