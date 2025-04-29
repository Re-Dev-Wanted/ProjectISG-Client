#pragma once

#include "DiaryStruct.generated.h"

USTRUCT()
struct PROJECTISG_API FGenerateDiaryRequest
{
	GENERATED_BODY()

	FString session_id;
	FString user_id;
	FString ingame_date;
};
