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
	GETTER(TSoftObjectPtr<UWorld>, TrainLevel)

private:
	FString Question;
	FText Answer;
	bool Completed;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UWorld> TrainLevel;
};
