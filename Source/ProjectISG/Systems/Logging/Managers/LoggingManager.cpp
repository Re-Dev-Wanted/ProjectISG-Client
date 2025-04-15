#include "LoggingManager.h"

#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

ULoggingManager* ULoggingManager::Instance = nullptr;

void ULoggingManager::Init(const UWorld* World)
{
	if (!World)
	{
		return;
	}

	if (World)
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

void ULoggingManager::Flush()
{
	TArray<FLoggingQueueItem> ItemsToProcess = BeaconQueue;
	BeaconQueue.Empty();

	for (FLoggingQueueItem& Item : ItemsToProcess)
	{
		SendHttpRequest(Item);
	}
	FPlatformProcess::Sleep(0.01f);
}

void ULoggingManager::QueueLogging(const FLoggingData& Payload)
{
	if (BeaconQueue.Num() >= MaxQueueSize)
	{
		// 오래된 내용을 보내고 제거하기
		SendHttpRequest(BeaconQueue[0]);
		BeaconQueue.RemoveAt(0);
	}

	FLoggingQueueItem Item;
	Item.Url = ApiPath;
	FJsonObjectConverter::UStructToJsonObjectString(Payload, Item.Payload);
	Item.RetryCount = 3;

	BeaconQueue.Add(Item);
}

void ULoggingManager::SendLoggingNow(const FLoggingData& Payload)
{
	FLoggingQueueItem Item;
	Item.Url = ApiPath;
	FJsonObjectConverter::UStructToJsonObjectString(Payload, Item.Payload);
	Item.RetryCount = 3;

	SendHttpRequest(Item);
}

void ULoggingManager::SendHttpRequest(FLoggingQueueItem& Item)
{
	const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request =
		FHttpModule::Get().
		CreateRequest();
	Request->SetURL(TEXT("http://localhost:3000/api/test"));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(
		TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	Request->SetContentAsString(Item.Payload);

	Request->OnProcessRequestComplete().BindLambda(
		[this, Item](const FHttpRequestPtr& RequestPtr,
		             const FHttpResponsePtr& Response,
		             const bool bWasSuccessful) mutable
		{
			const bool bValid = bWasSuccessful && Response.IsValid() &&
				EHttpResponseCodes::IsOk(Response->GetResponseCode());
			if (!bValid)
			{
				Item.RetryCount--;

				if (Item.RetryCount > 0)
				{
					BeaconQueue.Add(Item);
				}
			}
		});

	Request->ProcessRequest();
}
