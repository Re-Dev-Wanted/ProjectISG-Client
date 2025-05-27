#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "UIC_AutoQuestDialogueWidget.generated.h"

UCLASS()
class PROJECTISG_API UUIC_AutoQuestDialogueWidget : public UBaseUIController
{
	GENERATED_BODY()

public:
	void StartQuestDialogue();

	void SkipQuestDialogue();

protected:
	virtual void InitializeController(UBaseUIView* NewView,
	                                  UBaseUIModel* NewModel) override;

private:
	void InitializeDialogue();

	UFUNCTION()
	void OnFinishDialogue();
};
