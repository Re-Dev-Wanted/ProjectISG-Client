#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "LoggingManager.generated.h"

USTRUCT()
struct PROJECTISG_API FLoggingData
{
	GENERATED_BODY()

	UPROPERTY()
	uint16 Id;
};

struct FLoggingQueueItem
{
	FString Url;
	FString Payload;
	int32 RetryCount = 3;
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

	void Init(const UWorld* World);

	void SendLoggingNow(const FLoggingData& Payload);
	void QueueLogging(const FLoggingData& Payload);

	UFUNCTION()
	void Flush();

private:
	void SendHttpRequest(FLoggingQueueItem& Item);

	static ULoggingManager* Instance;

	TArray<FLoggingQueueItem> BeaconQueue;
	FTimerHandle FlushTimerHandle;
	float FlushIntervalSeconds = 300.0f;
	int32 MaxQueueSize = 50;
	FString ApiPath = TEXT("http://localhost:3000/api/test");
};
