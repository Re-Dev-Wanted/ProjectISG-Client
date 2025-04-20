#pragma once
#include "LoggingStruct.generated.h"

enum class ELoggingActionName : uint8;
enum class ELoggingActionType : uint8;

USTRUCT()
struct PROJECTISG_API FDiaryLogParams
{
	GENERATED_BODY()

	// 인게임 시간 정보
	UPROPERTY()
	FString CurrentDate;

	// 현재 위치
	// TODO: Enum화 대비
	UPROPERTY()
	FString Location;

	// 액션 타입
	UPROPERTY()
	ELoggingActionType ActionType;

	// 액션 이름
	UPROPERTY()
	ELoggingActionName ActionName;

	// 별도의 사설 데이터 구조체를 String화 시킨 값을 넣어줘야 함.
	UPROPERTY()
	FString Detail;

	// 상호작용한 사람 닉네임
	UPROPERTY()
	FString With;

	// 이미지 파일
	UPROPERTY()
	TArray<uint8> File;
};

USTRUCT()
struct PROJECTISG_API FDiaryLogData
{
	GENERATED_BODY()

	// 세션 ID
	UPROPERTY()
	FString session_id;

	// 현재 유저의 ID
	UPROPERTY()
	FString user_id;

	// 실제 시간 정보
	UPROPERTY()
	FString timestamp = FDateTime::Now().ToString();

	// 인게임 시간 정보
	UPROPERTY()
	FString ingame_datetime;

	// 현재 위치
	// TODO: Enum화 대비
	UPROPERTY()
	FString location;

	// 액션 타입
	UPROPERTY()
	ELoggingActionType action_type;

	// 액션 이름
	UPROPERTY()
	ELoggingActionName action_name;

	// 별도의 사설 데이터 구조체를 String화 시킨 값을 넣어줘야 함.
	UPROPERTY()
	FString detail;

	// 상호작용한 사람 닉네임
	UPROPERTY()
	FString with;

	// 이미지 파일
	UPROPERTY()
	TArray<uint8> file;
};

struct FApiCallData
{
	FString Url;
	FDiaryLogParams Payload;
	int32 RetryCount = 3;
};
