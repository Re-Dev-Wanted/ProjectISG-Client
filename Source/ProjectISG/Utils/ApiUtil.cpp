#include "ApiUtil.h"

#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"

TUniquePtr<FApiUtil> FApiUtil::MainAPI = nullptr;

FApiUtil* FApiUtil::GetMainAPI()
{
	if (!MainAPI.IsValid())
	{
		// 고유한 Unique ptr 생성
		MainAPI = MakeUnique<FApiUtil>();
		MainAPI->URL = TEXT(
			"http://192.168.10.96:8016/service2");
	}

	return MainAPI.Get();
}

void FApiUtil::GetApi(UObject* Caller, const FApiRequest& Request
                      , FApiResponse& Response) const
{
	SendRequest(Caller, TEXT("GET"), Request, Response);
}

void FApiUtil::PostApi(UObject* Caller, const FApiRequest& Request
                       , FApiResponse& Response) const
{
	SendRequest(Caller, TEXT("POST"), Request, Response);
}

void FApiUtil::SendRequest(UObject* Caller, const FString& Verb
                           , const FApiRequest& Request
                           , FApiResponse& Response) const
{
	if (Response.bIsLoading)
	{
		return;
	}

	const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest =
		FHttpModule::Get().CreateRequest();
	HttpRequest->SetURL(URL + Request.Path);
	HttpRequest->SetVerb(Verb);

	HttpRequest->SetTimeout(300.f);

	// Default Header
	if (!Request.Header.Contains(TEXT("Content-type")))
	{
		HttpRequest->SetHeader(TEXT("Content-type"), TEXT("application/json"));
	}

	// Custom Headers
	for (const auto& Pair : Request.Header)
	{
		HttpRequest->SetHeader(Pair.Key, Pair.Value);
	}

	if (Verb != TEXT("GET"))
	{
		HttpRequest->SetContentAsString(Request.Params);

		UE_LOG(LogTemp, Display, TEXT("%s: Call Data Request: %s")
		       , *Request.Path, *Request.Params);
	}

	Response.bIsLoading = true;

	// 값 자체를 그대로 활용하지 않고 복제해서 사용하기.
	const FApiRequest CapturedRequest = Request;
	FApiResponse* CapturedResponse = &Response;

	TWeakObjectPtr<UObject> WeakCaller = Caller;

	HttpRequest->OnProcessRequestComplete().BindLambda(
		[WeakCaller, CapturedRequest, CapturedResponse](
		const FHttpRequestPtr& Req, const FHttpResponsePtr& Res, bool bSuccess)
		{
			if (!WeakCaller.IsValid())
			{
				UE_LOG(LogTemp, Error
				       , TEXT("API Request caller no longer valid"));
				return;
			}

			CapturedResponse->bIsLoading = false;

			if (!bSuccess)
			{
				UE_LOG(LogTemp, Error, TEXT("알 수 없는 이유로 API Call 실패"))
				return;
			}

			UE_LOG(LogTemp, Display, TEXT("Content Body Data: %s")
			       , *Res->GetContentAsString())

			if (CapturedRequest.Callback.IsBound())
			{
				CapturedRequest.Callback.Execute(Req, Res, bSuccess);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("API Callback not bound."));
			}
		});

	HttpRequest->ProcessRequest();
}
