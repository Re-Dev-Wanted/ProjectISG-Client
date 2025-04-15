#include "LoggingManager.h"

#include "JsonObjectConverter.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

#include "ProjectISG/Utils/ApiUtil.h"

ULoggingManager* ULoggingManager::Instance = nullptr;

void ULoggingManager::LoggingToQueue(const FLoggingData& LoggingData)
{
	FLoggingQueueInfo NewLoggingQueueInfo;
	NewLoggingQueueInfo.Data = LoggingData;
	NewLoggingQueueInfo.RemainTime = DefaultRemainTime;

	LoggingDataQueue.Push(NewLoggingQueueInfo);
}

void ULoggingManager::LoggingToImmediately(const FLoggingData& LoggingData)
{
	FLoggingQueueInfo NewLoggingQueueInfo;
	NewLoggingQueueInfo.Data = LoggingData;
	NewLoggingQueueInfo.RemainTime = DefaultRemainTime;

	CallLogApi(NewLoggingQueueInfo);
}

void ULoggingManager::Flush()
{
	for (FLoggingQueueInfo DataQueue : LoggingDataQueue)
	{
		CallLogApi(DataQueue);
	}
}

void ULoggingManager::CallLogApi(FLoggingQueueInfo& Data)
{
	FApiRequest NewRequest;
	FApiResponse NewResponse;

	NewRequest.Path = TEXT("/api/test");

	FString Params;
	FJsonObjectConverter::UStructToJsonObjectString(Data.Data, Params);
	NewRequest.Params = Params;

	Data.RemainTime -= 1;
	TWeakObjectPtr<ULoggingManager> WeakThis = this;
	NewRequest.Callback = [WeakThis, &Data](const FHttpRequestPtr& Req,
	                                        const FHttpResponsePtr& Res,
	                                        const bool IsSuccess)
	{
		if (!WeakThis.IsValid())
		{
			return;
		}

		if (!WeakThis.Get())
		{
			return;
		}

		if (!IsSuccess)
		{
			if (Data.RemainTime == 0)
			{
				WeakThis.Get()->LoggingDataQueue.RemoveAtSwap(0);
				WeakThis.Get()->IsLoading = false;
				return;
			}

			WeakThis.Get()->CallLogApi(Data);
			return;
		}

		const FString JsonString = Res->GetContentAsString();
		WeakThis.Get()->LoggingDataQueue.RemoveAtSwap(0);
		WeakThis.Get()->IsLoading = false;
	};

	FApiUtil::GetMainAPI()->PostApi(
		this, NewRequest, NewResponse);
}
