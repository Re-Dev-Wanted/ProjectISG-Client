#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "LoggingManager.generated.h"

USTRUCT()
struct PROJECTISG_API FLoggingData
{
	GENERATED_BODY()

	UPROPERTY()
	uint8 Id;
};

USTRUCT()
struct PROJECTISG_API FLoggingQueueInfo
{
	GENERATED_BODY()

	UPROPERTY()
	FLoggingData Data;

	UPROPERTY()
	uint8 RemainTime;
};

UCLASS()
class PROJECTISG_API ULoggingManager : public UObject
{
	GENERATED_BODY()

public:
	static ULoggingManager* Get()
	{
		if (!Instance)
		{
			Instance = NewObject<ULoggingManager>();
			// Garbage Collector 에서 제외되게 처리함
			Instance->AddToRoot();
		}

		return Instance;
	};

	void LoggingToQueue(const FLoggingData& LoggingData);
	void LoggingToImmediately(const FLoggingData& LoggingData);

	void Flush();

private:
	static ULoggingManager* Instance;

	uint8 DefaultRemainTime = 3;
	bool IsLoading = false;

	TArray<FLoggingQueueInfo> LoggingDataQueue;

	void CallLogApi(FLoggingQueueInfo& Data);
};
