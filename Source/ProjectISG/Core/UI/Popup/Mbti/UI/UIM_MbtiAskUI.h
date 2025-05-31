#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIModel.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "UIM_MbtiAskUI.generated.h"

UCLASS()
class PROJECTISG_API UUIM_MbtiAskUI : public UBaseUIModel
{
	GENERATED_BODY()

public:
	GETTER_SETTER(FString, Question)
	GETTER_SETTER(FText, Answer)
	GETTER_SETTER(bool, Completed)
	GETTER_SETTER(uint8, MaxQuestionNum)
	GETTER_SETTER(uint8, CurrentQuestNum)
	GETTER_SETTER(float, CurrentPercentValue)
	GETTER(TSoftObjectPtr<UWorld>, TrainLevel)

private:
	FText Answer;
	bool Completed;
	FString Question;
	uint8 MaxQuestionNum = 7;
	uint8 CurrentQuestNum;
	float CurrentPercentValue;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UWorld> TrainLevel;
};
