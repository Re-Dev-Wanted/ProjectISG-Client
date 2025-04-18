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
	void SendLoggingNow(const FDiaryLogData& Payload);
	void QueueLogging(const FDiaryLogData& Payload);

	UFUNCTION()
	void Flush();

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	FString ApiPath = TEXT("http://192.168.10.70:8016/log");

	void SendHttpRequest(FApiCallData& CallData);

	void CreateLogDataStringForMultipart(const FDiaryLogData& LogData,
	                                     TArray<uint8>& Payload);

	FString Boundary;

	TArray<FDiaryLogData> BeaconQueue;

	FTimerHandle FlushTimerHandle;

	float FlushIntervalSeconds = 300.0f;
	int32 MaxQueueSize = 50;
};
