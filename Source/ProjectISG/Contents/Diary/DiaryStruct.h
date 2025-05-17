#pragma once

#include "DiaryStruct.generated.h"

USTRUCT()
struct PROJECTISG_API FGenerateDiaryRequest
{
	GENERATED_BODY()

	UPROPERTY()
	FString session_id;
	UPROPERTY()
	FString user_id;
	UPROPERTY()
	FString ingame_date;
};

USTRUCT()
struct PROJECTISG_API FSaveDiaryRequest
{
	GENERATED_BODY()

	UPROPERTY()
	FString session_id;
	UPROPERTY()
	FString user_id;
	UPROPERTY()
	FString ingame_date;
	UPROPERTY()
	FString diary_content;
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

	UPROPERTY()
	FString best_screenshot_filename;
};

USTRUCT()
struct PROJECTISG_API FDiaryData
{
	GENERATED_BODY()

	UPROPERTY()
	int diary_id = 0;

	UPROPERTY()
	FString ingame_datetime;

	UPROPERTY()
	FString content;

	UPROPERTY()
	FString best_screenshot_filename;
};

USTRUCT()
struct PROJECTISG_API FGetAllDiariesRequest
{
	GENERATED_BODY()

	UPROPERTY()
	FString user_id;

	UPROPERTY()
	FString session_id;
};

USTRUCT()
struct PROJECTISG_API FGetAllDiariesResponse
{
	GENERATED_BODY()

	UPROPERTY()
	FString user_id;

	UPROPERTY()
	FString session_id;

	UPROPERTY()
	TArray<FDiaryData> diaries;
};

USTRUCT()
struct PROJECTISG_API FPostMbtiAskParams
{
	GENERATED_BODY()

	UPROPERTY()
	FString user_id;

	UPROPERTY()
	FString session_id;
};

USTRUCT()
struct PROJECTISG_API FPostMbtiAskResponse
{
	GENERATED_BODY()

	UPROPERTY()
	FString question;

	UPROPERTY()
	FString dimension;

	UPROPERTY()
	bool completed = false;
};

USTRUCT()
struct PROJECTISG_API FPostMbtiAnswerkParams
{
	GENERATED_BODY()

	UPROPERTY()
	FString user_id;

	UPROPERTY()
	FString session_id;

	UPROPERTY()
	FString response;
};

USTRUCT()
struct PROJECTISG_API FPostMbtiAnswerResponse
{
	GENERATED_BODY()

	UPROPERTY()
	bool completed = false;
};
