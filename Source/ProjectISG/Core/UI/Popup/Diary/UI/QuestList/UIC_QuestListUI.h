#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "UIC_QuestListUI.generated.h"

class UInputAction;
struct FQuestRequireData;

UCLASS()
class PROJECTISG_API UUIC_QuestListUI : public UBaseUIController
{
	GENERATED_BODY()

public:
	void SetQuestInfo(const FString& QuestId);

protected:
	virtual void AppearUI() override;

	virtual void InitializeController(UBaseUIView* NewView
									, UBaseUIModel* NewModel) override;

	virtual void
	BindInputAction(UEnhancedInputComponent* InputComponent) override;

private:
	void InitializeQuestList();

	void SetQuestInfoData(const FString& QuestId);

	void SetQuestRequireData(const FString& QuestId);

	void SetQuestRewardData(const FString& QuestId);

	void SetQuestRequireItemData(FQuestRequireData& RequireQuest) const;

	UFUNCTION()
	void OnClickQuestButton();

	UPROPERTY(EditDefaultsOnly, Category = "Options|Input"
		, meta = (AllowPrivateAccess = true, EditCondition = "IsInputAccess"))
	TObjectPtr<UInputAction> ExitActionInput;
};
