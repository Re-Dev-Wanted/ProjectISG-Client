#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LoggingSubSystem.generated.h"

USTRUCT()
struct PROJECTISG_API FDiaryLogData
{
	GENERATED_BODY()

	UPROPERTY()
	uint16 Id;
};

struct FApiCallData
{
	FString Url;
	FString Payload;
	int32 RetryCount = 3;
};

UCLASS()
class PROJECTISG_API ULoggingSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void SendLoggingNow(const FDiaryLogData& Payload);
	void QueueLogging(const FDiaryLogData& Payload);

	UFUNCTION()
	void Flush();

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	FString ApiPath = TEXT("http://192.168.20.108:8016/log");

	void SendHttpRequest(FApiCallData& CallData);

	TArray<FDiaryLogData> BeaconQueue;
	FTimerHandle FlushTimerHandle;
	float FlushIntervalSeconds = 300.0f;
	int32 MaxQueueSize = 50;
};
