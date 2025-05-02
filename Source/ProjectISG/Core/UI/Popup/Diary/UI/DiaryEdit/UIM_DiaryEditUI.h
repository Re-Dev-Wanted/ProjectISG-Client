#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIModel.h"
#include "ProjectISG/Utils/ApiUtil.h"
#include "UIM_DiaryEditUI.generated.h"

UCLASS()
class PROJECTISG_API UUIM_DiaryEditUI : public UBaseUIModel
{
	GENERATED_BODY()

public:
	FApiResponse SaveDiaryResponse;
};
