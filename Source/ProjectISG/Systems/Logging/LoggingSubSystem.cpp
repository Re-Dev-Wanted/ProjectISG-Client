#include "LoggingSubSystem.h"

#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "ProjectISG/Utils/EnumUtil.h"

void ULoggingSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	Boundary = "----Boundary" + FGuid::NewGuid().ToString(
		EGuidFormats::Digits);
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
	FApiCallData NewApiCallData;

	NewApiCallData.Url = TEXT("/upload_with_screenshot");
	NewApiCallData.Payload = Payload;
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
	FApiCallData NewApiCallData;

	TArray<FDiaryLogData> DataList = BeaconQueue;
	BeaconQueue.Empty();

	for (const FDiaryLogData& DiaryLogData : DataList)
	{
		NewApiCallData.Url = TEXT("/upload_with_screenshot");
		NewApiCallData.Payload = DiaryLogData;
		NewApiCallData.RetryCount = 3;
		SendHttpRequest(NewApiCallData);
	}
}

void ULoggingSubSystem::CreateLogDataStringForMultipart(
	const FDiaryLogData& LogData, TArray<uint8>& Payload)
{
	FString LineBreak = "\r\n";

	// JSON 객체 구성
	TSharedPtr<FJsonObject> Obj = MakeShared<FJsonObject>();
	Obj->SetStringField("session_id", LogData.session_id);
	Obj->SetStringField("user_id", LogData.user_id);
	Obj->SetStringField("timestamp", LogData.timestamp);
	Obj->SetStringField("ingame_datetime", LogData.ingame_datetime);
	Obj->SetStringField("location", LogData.location);
	Obj->SetStringField("action_type",
	                    StaticEnum<ELoggingActionType>()->GetNameStringByValue(
		                    static_cast<int64>(LogData.action_type)));
	Obj->SetStringField("action_name",
	                    StaticEnum<ELoggingActionName>()->GetNameStringByValue(
		                    static_cast<int64>(LogData.action_name)));
	Obj->SetStringField("detail", LogData.detail);
	Obj->SetStringField("with", LogData.with);
	Obj->SetStringField("file", "file"); // 파일 키

	FString JsonPayload;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(
		&JsonPayload);
	FJsonSerializer::Serialize(Obj.ToSharedRef(), Writer);

	// JSON part
	FString JsonPart =
		"--" + Boundary + LineBreak +
		"Content-Disposition: form-data; name=\"log\"" + LineBreak +
		"Content-Type: application/json" + LineBreak + LineBreak +
		JsonPayload + LineBreak;

	FTCHARToUTF8 JsonConv(*JsonPart);
	Payload.Append((uint8*)(JsonConv.Get()), JsonConv.Length());


	// File part
	FString FileHeader =
		"--" + Boundary + LineBreak +
		"Content-Disposition: form-data; name=\"file\"; filename=\"log_image.png\""
		+ LineBreak +
		"Content-Type: application/octet-stream" + LineBreak + LineBreak;

	FTCHARToUTF8 HeaderConv(*FileHeader);
	Payload.Append((uint8*)(HeaderConv.Get()), HeaderConv.Length());

	Payload.Append(LogData.file);

	FTCHARToUTF8 BreakConv(*LineBreak);
	Payload.Append((uint8*)BreakConv.Get(), BreakConv.Length());

	FString Closing = "--" + Boundary + "--" + LineBreak;
	FTCHARToUTF8 ClosingConv(*Closing);
	Payload.Append((uint8*)(ClosingConv.Get()),
	               ClosingConv.Length());
}

void ULoggingSubSystem::SendHttpRequest(FApiCallData& CallData)
{
	const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request =
		FHttpModule::Get().
		CreateRequest();

	TArray<uint8> Payload;

	Request->SetURL(ApiPath + CallData.Url);
	Request->SetVerb(TEXT("POST"));

	FString ContentType = "multipart/form-data; boundary=" + Boundary;

	CreateLogDataStringForMultipart(CallData.Payload, Payload);
	Request->SetContent(Payload);

	Request->OnProcessRequestComplete().BindLambda([this, CallData]
	(const FHttpRequestPtr& Req, const FHttpResponsePtr& Res,
	 const bool bSuccess) mutable
		{
			const bool bValid = bSuccess &&
				EHttpResponseCodes::IsOk(Res->GetResponseCode());

			UE_LOG(LogTemp, Display, TEXT("데이터 성공 여부: %d"),
			       Res->GetResponseCode())

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

			// TODO: 만약 계속해서 서버에 문제가 있는 경우에
			// 잘못하면 로그데이터가 무한정 쌓일 수 있는 구조
			BeaconQueue.Add(CallData.Payload);
		});

	Request->ProcessRequest();
}
