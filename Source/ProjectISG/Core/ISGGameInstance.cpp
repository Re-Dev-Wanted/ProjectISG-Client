#include "ISGGameInstance.h"

#include "Controller/LobbyPlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/IHttpResponse.h"
#include "ProjectISG/Contents/Cooking/Managers/CookingManager.h"
#include "ProjectISG/Contents/Crafting/Managers/CraftingManager.h"
#include "ProjectISG/Contents/Farming/Managers/FarmingManager.h"
#include "ProjectISG/Contents/Fishing/Managers/FishingManager.h"
#include "ProjectISG/Contents/Trading/TradingManager.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"
#include "ProjectISG/Systems/QuestStory/Manager/QuestStoryManager.h"
#include "ProjectISG/Utils/ApiUtil.h"
#include "ProjectISG/Utils/SessionUtil.h"

class ALobbyPlayerController;

UISGGameInstance::UISGGameInstance()
{
	UItemManager::Initialize();
	UCookingManager::Initialize();
	UTradingManager::Initialize();
	UFishingManager::Initialize();
	UFarmingManager::Initialize();
	UCraftingManager::Initialize();
	UQuestStoryManager::Initialize();
}

void UISGGameInstance::CreateGameSessionIdAndCreateSession()
{
	FApiRequest Request;
	Request.Path = TEXT("/diary/new_session");

	FString DataParams = TEXT("{}");

	Request.Callback.BindLambda(
		[this](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bSuccess)
		{
			if (bSuccess && Res.IsValid() && Res->GetResponseCode() == 200)
			{
				FString ResponseString = Res->GetContentAsString();
				UE_LOG(LogTemp, Warning, TEXT("세션 생성 성공. 원본 응답: %s"),
				       *ResponseString);

				// JSON 문자열을 파싱하기 위한 준비
				TSharedPtr<FJsonObject> JsonObject;
				TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(
					ResponseString);

				if (FJsonSerializer::Deserialize(Reader, JsonObject) &&
					JsonObject.IsValid())
				{
					// "session_id" 키로 문자열 값을 가져옴
					FString ExtractedSessionId;
					if (JsonObject->TryGetStringField(
						TEXT("session_id"), ExtractedSessionId))
					{
						SessionId = ExtractedSessionId;
						SessionCreateSuccess = true;
						UE_LOG(LogTemp, Warning, TEXT("파싱된 새 세션 ID: %s"),
						       *SessionId);
						CreateSession();
					}
					else
					{
						UE_LOG(LogTemp, Error,
						       TEXT(
							       "JSON에서 'session_id' 필드를 찾거나 문자열로 가져올 수 없습니다."
						       ));
					}
				}
				else
				{
					UE_LOG(LogTemp, Error,
					       TEXT(
						       "응답 JSON 파싱 실패 (FJsonSerializer::Deserialize 실패): %s"
					       ), *ResponseString);
				}
			}
			else if (Res.IsValid())
			{
				UE_LOG(LogTemp, Error, TEXT("세션 생성 실패. 응답 코드: %d, 내용: %s"),
				       Res->GetResponseCode(), *Res->GetContentAsString());
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("세션 생성 요청 실패. 응답 없음."));
			}
		});

	FApiResponse Response;
	FApiUtil::GetMainAPI()->PostApi(this, Request, Response);
}

void UISGGameInstance::CreateSession()
{
	UE_LOG(LogTemp, Warning, TEXT("세션을 생성합니다."));
	FSessionCreateData CreateData;
	CreateData.MaxPlayer = 4;
	FString RoomName = FString::Printf(
		TEXT("%s 님의 농장"),
		*GetWorld()->GetFirstPlayerController()->GetPlayerState<APlayerState>()
		           ->GetPlayerName());
	CreateData.RoomName = RoomName;
	CreateData.OnCreateSessionCompleteDelegate =
		FOnCreateSessionCompleteDelegate::CreateUObject(
			this, &ThisClass::OnCompleteCreate);

	FSessionUtil::CreateSession(CreateData);
}

void UISGGameInstance::OnCompleteCreate(FName SessionName, bool IsCreateSuccess)
{
	if (IsCreateSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("세션을 만들었습니다."));
		GetWorld()->ServerTravel(
			FString("/Game/Maps/MainPlayLevel/MainPlayLevel?listen"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("세션을 만드는 것에 실패하였습니다."));
	}
}

void UISGGameInstance::JoinFoundSession()
{
	ALobbyPlayerController* LobbyPlayerController = Cast<
		ALobbyPlayerController>(GetWorld()->GetFirstPlayerController());

	OnJoinSessionCompleteDelegate =
		FOnJoinSessionCompleteDelegate::CreateUObject(
			this, &ThisClass::OnJoinSession);

	FSessionUtil::JoinSession(GetWorld(),
	                          LobbyPlayerController->SessionSearchResult,
	                          OnJoinSessionCompleteDelegate);
}

void UISGGameInstance::OnJoinSession(FName SessionName,
                                     EOnJoinSessionCompleteResult::Type Type)
{
	FString Address;
	if (FSessionUtil::OnlineSessionInterface->GetResolvedConnectString(
		NAME_GameSession, Address))
	{
		GetFirstLocalPlayerController()->ClientTravel(Address, TRAVEL_Absolute);
	}
}

void UISGGameInstance::Init()
{
	Super::Init();

	FSessionUtil::Init();
}
