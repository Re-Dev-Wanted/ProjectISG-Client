#include "LoggingSubSystem.h"

#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

void ULoggingSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (const UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(
			FlushTimerHandle,
			this,
			&ThisClass::Flush,
			FlushIntervalSeconds,
			true
		);
	}
}

void ULoggingSubSystem::Deinitialize()
{
	if (const UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(FlushTimerHandle);
	}

	Super::Deinitialize();
}

void ULoggingSubSystem::SendLoggingNow(const FDiaryLogData& Payload)
{
	TArray<FDiaryLogData> PayloadData;
	PayloadData.Add(Payload);

	const FString LogData;
	FJsonObjectConverter::JsonArrayStringToUStruct(LogData, &PayloadData);

	FApiCallData NewApiCallData;

	NewApiCallData.Url = TEXT("/upload_with_screenshot");
	NewApiCallData.Payload = LogData;
	NewApiCallData.RetryCount = 3;

	SendHttpRequest(NewApiCallData);
}

void ULoggingSubSystem::QueueLogging(const FDiaryLogData& Payload)
{
	if (BeaconQueue.Num() >= MaxQueueSize)
	{
		Flush();
	}

	BeaconQueue.Add(Payload);
}

void ULoggingSubSystem::Flush()
{
	const FString LogData;
	FJsonObjectConverter::JsonArrayStringToUStruct(LogData, &BeaconQueue);
	BeaconQueue.Empty();

	FApiCallData NewApiCallData;

	NewApiCallData.Url = TEXT("/upload_with_screenshot");
	NewApiCallData.Payload = LogData;
	NewApiCallData.RetryCount = 3;

	SendHttpRequest(NewApiCallData);
}

void ULoggingSubSystem::SendHttpRequest(FApiCallData& CallData)
{
	const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request =
		FHttpModule::Get().
		CreateRequest();

	Request->SetURL(ApiPath + CallData.Url);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(
		TEXT("Content-Type"),
		TEXT("application/x-www-form-urlencoded"));
	Request->SetContentAsString(CallData.Payload);

	Request->OnProcessRequestComplete().BindLambda([this, CallData]
	(const FHttpRequestPtr& Req, const FHttpResponsePtr& Res,
	 const bool bSuccess) mutable
		{
			const bool bValid = bSuccess && Res.IsValid() &&
				EHttpResponseCodes::IsOk(Res->GetResponseCode());

			if (bValid)
			{
				return;
			}

			CallData.RetryCount -= 1;
			if (CallData.RetryCount > 0)
			{
				SendHttpRequest(CallData);
				return;
			}

			// 만약 계속해서 서버에 문제가 있는 경우에
			// 잘못하면 로그데이터가 무한정 쌓일 수 있는 구조
			TArray<TSharedPtr<FJsonValue>> JsonArray;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(
				CallData.Payload);

			if (!FJsonSerializer::Deserialize(Reader, JsonArray))
			{
				return;
			}

			for (const TSharedPtr<FJsonValue>& JsonValue : JsonArray)
			{
				FDiaryLogData DataItem;
				if (FJsonObjectConverter::JsonObjectToUStruct(
					JsonValue->AsObject().ToSharedRef(),
					FDiaryLogData::StaticStruct(),
					&DataItem,
					0, 0))
				{
					BeaconQueue.Add(DataItem);
				}
			}
		});
}
