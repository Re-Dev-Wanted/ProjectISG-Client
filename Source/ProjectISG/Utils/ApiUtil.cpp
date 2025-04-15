#include "ApiUtil.h"

#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Interfaces/IHttpRequest.h"

FApiUtil* FApiUtil::MainAPI;

void FApiUtil::GetApi(UObject* Caller, const FApiRequest& Request,
                      FApiResponse& Response) const
{
	if (Response.IsLoading)
	{
		return;
	}

	const FHttpRequestRef HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetURL(FString::Printf(TEXT("%s%s"), *URL, *Request.Path));
	HttpRequest->SetVerb("GET");
	HttpRequest->SetHeader(TEXT("Content-type"), TEXT("application/json"));

	for (auto NewHeader : Request.Header)
	{
		HttpRequest->SetHeader(NewHeader.Key, NewHeader.Value);
	}

	Response.IsLoading = true;

	TWeakObjectPtr<UObject> WeakThis = Caller;
	HttpRequest->OnProcessRequestComplete().BindLambda(
		[WeakThis, &Request, &Response](const FHttpRequestPtr& Req,
		                                const FHttpResponsePtr& Res,
		                                bool bProcessedSuccessfully)
		{
			if (!WeakThis.IsValid())
			{
				UE_LOG(LogTemp, Error, TEXT("Memory Error!"))
				return;
			}
			Response.IsLoading = false;
			Request.Callback(Req, Res, bProcessedSuccessfully);
		});

	HttpRequest->ProcessRequest();
}

void FApiUtil::PostApi(UObject* Caller, const FApiRequest& Request,
                       FApiResponse& Response) const
{
	if (Response.IsLoading)
	{
		return;
	}

	const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest =
		FHttpModule::Get().CreateRequest();

	HttpRequest->SetURL(FString::Printf(TEXT("%s%s"), *URL, *Request.Path));
	HttpRequest->SetVerb("POST");
	HttpRequest->SetContentAsString(Request.Params);

	HttpRequest->SetHeader(TEXT("Content-type"), TEXT("application/json"));

	for (auto NewHeader : Request.Header)
	{
		HttpRequest->SetHeader(NewHeader.Key, NewHeader.Value);
	}

	Response.IsLoading = true;

	TWeakObjectPtr<UObject> WeakThis = Caller;
	HttpRequest->OnProcessRequestComplete().BindLambda(
		[WeakThis, &Request, &Response](const FHttpRequestPtr& Req,
		                                const FHttpResponsePtr& Res,
		                                bool bProcessedSuccessfully)
		{
			if (!WeakThis.IsValid())
			{
				UE_LOG(LogTemp, Error, TEXT("Memory Error!"))
				return;
			}
			Response.IsLoading = false;
			Request.Callback(Req, Res, bProcessedSuccessfully);
		});

	HttpRequest->ProcessRequest();
}
