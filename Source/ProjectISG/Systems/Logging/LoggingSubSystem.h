#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LoggingStruct.h"
#include "LoggingSubSystem.generated.h"

UCLASS()
class PROJECTISG_API ULoggingSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void LoggingData(FDiaryLogParams& Payload);
	void SendLoggingNow(const FDiaryLogParams& Payload);
	void QueueLogging(const FDiaryLogParams& Payload);

	UFUNCTION()
	void Flush();

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	uint8 MaxScreenShotLogCount = 10;
	uint8 CurrentScreenShotLogCount = 1;

	FString ApiPath = TEXT(
		"http://192.168.10.96:8016/service2/log");

	void SendHttpRequest(FApiCallData& CallData);

	void CreateLogDataStringForMultipart(const FDiaryLogParams& LogData
	                                     , TArray<uint8>& Payload);

	FString Boundary;

	TArray<FDiaryLogParams> BeaconQueue;

	FTimerHandle FlushTimerHandle;

	float FlushIntervalSeconds = 300.0f;
	int32 MaxQueueSize = 50;
};
