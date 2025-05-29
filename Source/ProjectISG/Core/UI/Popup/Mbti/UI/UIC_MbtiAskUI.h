#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "ProjectISG/Utils/ApiUtil.h"
#include "UIC_MbtiAskUI.generated.h"

UCLASS()
class PROJECTISG_API UUIC_MbtiAskUI : public UBaseUIController
{
	GENERATED_BODY()

public:
	virtual void OnPushUI() override;

protected:
	virtual void InitializeController(UBaseUIView* NewView,
	                                  UBaseUIModel* NewModel) override;

private:
	FApiResponse MbtiAskResponse;

	void AskNewMbti();

	UFUNCTION()
	void AnswerMbti();

	UFUNCTION()
	void OnChangeText(const FText& Text);

	UFUNCTION()
	void OnCommitText(const FText& Text,
	                  ETextCommit::Type CommitMethod);
};
