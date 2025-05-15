#include "MainGameState.h"

#include "GameFramework/PlayerState.h"
#include "Interfaces/IHttpResponse.h"
#include "Net/UnrealNetwork.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Utils/ApiUtil.h"

AMainGameState::AMainGameState()
{
}

void AMainGameState::GetLifetimeReplicatedProps(
	TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMainGameState, CurrentWorldQuestId)
	DOREPLIFETIME(AMainGameState, SessionId)
}

void AMainGameState::GetGameSessionId()
{
	FApiRequest Request;
	Request.Path = TEXT("/diary/new_session");

	FString DataParams = TEXT("{}");

	Request.Callback.BindLambda([this](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bSuccess)
  {
	  if (bSuccess && Res.IsValid() && Res->GetResponseCode() == 200)
	  {
		  FString ResponseString = Res->GetContentAsString();
		  UE_LOG(LogTemp, Warning, TEXT("세션 생성 성공. 원본 응답: %s"), *ResponseString);

		  // JSON 문자열을 파싱하기 위한 준비
		  TSharedPtr<FJsonObject> JsonObject;
		  TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);

		  if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
		  {
			  // "session_id" 키로 문자열 값을 가져옴
			  FString ExtractedSessionId;
			  if (JsonObject->TryGetStringField(TEXT("session_id"), ExtractedSessionId))
			  {
				  SessionId = ExtractedSessionId;
				  UE_LOG(LogTemp, Warning, TEXT("파싱된 새 세션 ID: %s"), *SessionId);
			  }
			  else
			  {
				  UE_LOG(LogTemp, Error, TEXT("JSON에서 'session_id' 필드를 찾거나 문자열로 가져올 수 없습니다."));
			  }
		  }
		  else
		  {
			  UE_LOG(LogTemp, Error, TEXT("응답 JSON 파싱 실패 (FJsonSerializer::Deserialize 실패): %s"), *ResponseString);
		  }
	  }
	  else if (Res.IsValid())
	  {
		  UE_LOG(LogTemp, Error, TEXT("세션 생성 실패. 응답 코드: %d, 내용: %s"), Res->GetResponseCode(), *Res->GetContentAsString());
	  }
	  else
	  {
		  UE_LOG(LogTemp, Error, TEXT("세션 생성 요청 실패. 응답 없음."));
	  }
  });

	FApiResponse Response;
	FApiUtil::GetMainAPI()->PostApi(this, Request, Response);
}

void AMainGameState::StartWorldQuest(const FString& QuestId)
{
	CurrentWorldQuestId = QuestId;

	for (const TObjectPtr<APlayerState> PlayerState : PlayerArray)
	{
		AMainPlayerController* PC = Cast<AMainPlayerController>(
			PlayerState->GetPlayerController());

		// 서버에서만 호출되는 이 함수가 LocalController인 경우는
		// 무조건 서버 Controller이고 나머지가 Client Controller라는 반증
		if (PC->IsLocalController())
		{
			PC->StartQuestToPlayer(QuestId);
		}
		else
		{
			PC->Client_StartQuestToPlayer(QuestId);
		}
	}
}
