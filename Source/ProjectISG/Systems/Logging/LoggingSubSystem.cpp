#include "LoggingSubSystem.h"

#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/GameMode/MainGameMode.h"
#include "ProjectISG/Core/GameMode/MainGameState.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Systems/Time/TimeManager.h"
#include "ProjectISG/Utils/EnumUtil.h"
#include "ProjectISG/Utils/SessionUtil.h"

void ULoggingSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	Boundary = TEXT("----Boundary") + FGuid::NewGuid().ToString(
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

void ULoggingSubSystem::LoggingData(FDiaryLogParams& Payload)
{
	// CurrentDate 강제 주입 처리
	const ATimeManager* TimeManager = Cast<ATimeManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(),
		                                  ATimeManager::StaticClass()));
	Payload.CurrentDate = TimeManager->GetDateText();

	QueueLogging(Payload);
}

void ULoggingSubSystem::LoggingDataWithScreenshot(FDiaryLogParams& Payload)
{
	const ATimeManager* TimeManager = Cast<ATimeManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(),
		                                  ATimeManager::StaticClass()));
	Payload.CurrentDate = TimeManager->GetDateText();

	if (FMath::RandRange(0, 1) > static_cast<double>(CurrentScreenShotLogCount)
		/ MaxScreenShotLogCount)
	{
		CurrentScreenShotLogCount = 1;
		SendLoggingNow(Payload);
		return;
	}

	QueueLogging(Payload);
	CurrentScreenShotLogCount += 1;
}

void ULoggingSubSystem::SendLoggingNow(const FDiaryLogParams& Payload)
{
	FApiCallData NewApiCallData;

	NewApiCallData.Url = TEXT("/upload_with_screenshot");
	NewApiCallData.Payload = Payload;
	NewApiCallData.RetryCount = 3;

	SendHttpRequest(NewApiCallData);
}

void ULoggingSubSystem::QueueLogging(const FDiaryLogParams& Payload)
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

	TArray<FDiaryLogParams> DataList = BeaconQueue;
	BeaconQueue.Empty();

	for (const FDiaryLogParams& DiaryLogData : DataList)
	{
		NewApiCallData.Url = TEXT("/upload_with_screenshot");
		NewApiCallData.Payload = DiaryLogData;
		NewApiCallData.RetryCount = 3;
		SendHttpRequest(NewApiCallData);
	}
}

void ULoggingSubSystem::CreateLogDataStringForMultipart(
	const FDiaryLogParams& LogData, TArray<uint8>& Payload)
{
	// 첫 번째 경계선에는 앞에 \r\n이 없어야 함
	const FString FirstBoundary = TEXT("--") + Boundary + TEXT("\r\n");
	// 이후 경계선에는 앞에 \r\n이 있어야 함
	const FString MiddleBoundary = TEXT("\r\n--") + Boundary + TEXT("\r\n");
	// 마지막 경계선
	const FString EndBoundary = TEXT("\r\n--") + Boundary + TEXT("--\r\n");

	// RequestContent 초기화 - 비워두기
	Payload.Empty();

	// 함수 형식 텍스트 필드 추가
	auto AddTextField = [&Payload, &FirstBoundary, &MiddleBoundary](
		const FString& FieldName, const FString& Value, const bool IsFirstField)
	{
		const FString BoundaryStr = IsFirstField
			                            ? FirstBoundary
			                            : MiddleBoundary;

		const FString FieldHeader = BoundaryStr + TEXT(
			"Content-Disposition: form-data; name=\"") + FieldName + TEXT(
			"\"\r\n\r\n");
		const FString FieldContent = FieldHeader + Value;

		const FTCHARToUTF8 Converter(*FieldContent);
		Payload.Append((uint8*)Converter.Get(), Converter.Length());
	};

	// 필수 필드 추가
	AddTextField(
		TEXT("session_id"),
		GetWorld()->GetGameState<AMainGameState>()->GetSessionId(), true);
	AddTextField(TEXT("user_id"), FSessionUtil::GetCurrentId(GetWorld()),
	             false);
	AddTextField(TEXT("timestamp"), FDateTime::Now().ToString(), false);
	AddTextField(TEXT("ingame_datetime"), LogData.CurrentDate, false);
	// TODO: 나중에 해당 값을 별도로 Parameter로 받던가
	// or 외부 상태를 관리하고 가져오는 형식으로 변경해줘야 한다.
	AddTextField(TEXT("location"), LogData.Location, false);
	AddTextField(
		TEXT("action_type"),
		FEnumUtil::GetClassEnumKeyAsString(LogData.ActionType), false);
	AddTextField(
		TEXT("action_name"),
		FEnumUtil::GetClassEnumKeyAsString(LogData.ActionName), false);
	AddTextField(TEXT("detail"), LogData.Detail, false);

	// 선택적 필드 추가
	if (!LogData.With.IsEmpty())
	{
		AddTextField(TEXT("with_"), LogData.With, false);
	}

	// 스크린샷 파일 추가
	if (LogData.File.Num() > 0)
	{
		const FString MimeType = TEXT("image/png");

		FString FileHeader = MiddleBoundary +
			TEXT("Content-Disposition: form-data; name=\"file\"; filename=\"") +
			TEXT("log_screenshot") + TEXT("\"\r\n") +
			TEXT("Content-Type: ") + MimeType + TEXT("\r\n\r\n");

		FTCHARToUTF8 HeaderConverter(*FileHeader);
		Payload.Append((uint8*)HeaderConverter.Get(), HeaderConverter.Length());
		Payload.Append(LogData.File);
	}

	// 마지막 경계선 추가
	FTCHARToUTF8 EndBoundaryConverter(*EndBoundary);
	Payload.Append((uint8*)EndBoundaryConverter.Get(),
	               EndBoundaryConverter.Length());
}

void ULoggingSubSystem::SendHttpRequest(FApiCallData& CallData)
{
	const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request =
		FHttpModule::Get().
		CreateRequest();

	TArray<uint8> Payload;
	const FString ContentType = "multipart/form-data; boundary=" + Boundary;
	CreateLogDataStringForMultipart(CallData.Payload, Payload);

	Request->SetURL(ApiPath + CallData.Url);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-type"), ContentType);
	Request->SetContent(Payload);

	Request->OnProcessRequestComplete().BindLambda([this, CallData]
	(const FHttpRequestPtr& Req, const FHttpResponsePtr& Res,
	 const bool bSuccess) mutable
		{
			// const bool bValid = bSuccess &&
			// 	EHttpResponseCodes::IsOk(Res->GetResponseCode());

			if (bSuccess)
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
