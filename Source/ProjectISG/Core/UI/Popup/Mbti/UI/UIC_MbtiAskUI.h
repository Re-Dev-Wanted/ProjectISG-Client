#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "ProjectISG/Utils/ApiUtil.h"
#include "UIC_MbtiAskUI.generated.h"

UCLASS()
class PROJECTISG_API UUIC_MbtiAskUI : public UBaseUIController
{
	GENERATED_BODY()

protected:
	virtual void AppearUI() override;

	virtual void InitializeController(UBaseUIView* NewView,
	                                  UBaseUIModel* NewModel) override;

private:
	void AskNewMbti();

	void AnswerMbti();

	FApiResponse MbtiAskResponse;

	UFUNCTION()
	void OnChangeText(const FText& Text);

	UFUNCTION()
	void OnCommitText(const FText& Text,
	                  ETextCommit::Type CommitMethod);
};
