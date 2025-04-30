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

USTRUCT()
struct PROJECTISG_API FGenerateDiaryResponse
{
	GENERATED_BODY()

	UPROPERTY()
	FString user_id;

	UPROPERTY()
	FDateTime date;

	UPROPERTY()
	FString mbti;

	UPROPERTY()
	TArray<FString> emotion_tags;

	UPROPERTY()
	TArray<FString> emotion_keywords;

	UPROPERTY()
	FString diary;
};
