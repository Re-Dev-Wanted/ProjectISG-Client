#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Contents/Diary/DiaryStruct.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIModel.h"
#include "ProjectISG/Utils/ApiUtil.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "UIM_DiaryListUI.generated.h"

UCLASS()
class PROJECTISG_API UUIM_DiaryListUI : public UBaseUIModel
{
	GENERATED_BODY()

public:
	FApiResponse GetAllDiariesResponse;
	FGetAllDiariesResponse DiaryData;
	GETTER_SETTER(uint32, CurrentDiaryIndex)

private:
	uint32 CurrentDiaryIndex;
};
