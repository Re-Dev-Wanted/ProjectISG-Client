// Fill out your copyright notice in the Description page of Project Settings.


#include "UIC_MultiPlay.h"

#include "OnlineSessionSettings.h"
#include "UIV_MultiPlay.h"
#include "Components/ScrollBox.h"
#include "ProjectISG/Core/UI/Popup/Lobby/RoomItem/UIV_RoomItem.h"
#include "GameFramework/PlayerState.h"
#include "ProjectISG/Utils/SessionUtil.h"


void UUIC_MultiPlay::CreateSession()
{
	UE_LOG(LogTemp, Error, TEXT("세션을 생성합니다."));

	FSessionCreateData CreateData;
	CreateData.MaxPlayer = MaxPlayer;
	FString RoomName = FString::Printf(
		TEXT("%s 님의 농장"),
		*GetPlayerController()->GetPlayerState<APlayerState>()->
		                        GetPlayerName());
	CreateData.RoomName = RoomName;
	CreateData.OnCreateSessionCompleteDelegate =
		FOnCreateSessionCompleteDelegate::CreateUObject(
			this, &ThisClass::OnCompleteCreate);
	FSessionUtil::CreateSession(CreateData);
}

void UUIC_MultiPlay::OnCompleteCreate(FName SessionName, bool IsCreateSuccess)
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

void UUIC_MultiPlay::SearchGameAndJoinSession()
{
	UE_LOG(LogTemp, Error, TEXT("세션을 검색합니다."));

	SessionSearchData.SessionSearch = MakeShared<FOnlineSessionSearch>();
	SessionSearchData.OnFindSessionsCompleteDelegate =
		FOnFindSessionsCompleteDelegate::CreateUObject(
			this, &ThisClass::OnCompleteSearchAndTryJoin);
	FSessionUtil::SearchSession(SessionSearchData);
}

void UUIC_MultiPlay::FindSession()
{
	UE_LOG(LogTemp, Error, TEXT("세션을 검색합니다."));

	SessionSearchData.SessionSearch = MakeShared<FOnlineSessionSearch>();
	SessionSearchData.OnFindSessionsCompleteDelegate =
		FOnFindSessionsCompleteDelegate::CreateUObject(
			this, &ThisClass::OnCompleteSearch);
	FSessionUtil::SearchSession(SessionSearchData);
}

void UUIC_MultiPlay::OnCompleteSearchAndTryJoin(bool IsSearchSuccess)
{
	if (IsSearchSuccess)
	{
		for (int i = 0; i < SessionSearchData.SessionSearch->SearchResults.Num()
		     ; i++)
		{
			UE_LOG(LogTemp, Error, TEXT("세션을 검색하는 것에 성공하였습니다."));
			OnJoinSessionCompleteDelegate =
				FOnJoinSessionCompleteDelegate::CreateUObject(
					this, &ThisClass::OnJoinSession);
			FSessionUtil::JoinSession(GetWorld(),
			                          SessionSearchData.SessionSearch->
			                          SearchResults[i],
			                          OnJoinSessionCompleteDelegate);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("세션을 검색하는 것에 실패하였습니다."));
	}
}

void UUIC_MultiPlay::OnCompleteSearch(bool IsSearchSuccess)
{
	if (IsSearchSuccess)
	{
		UE_LOG(LogTemp, Error, TEXT("세션을 검색하는 것에 성공하였습니다."));

		UUIV_MultiPlay* MultiPlayView = Cast<UUIV_MultiPlay>(GetView());
		for (int i = 0; i < SessionSearchData.SessionSearch->SearchResults.Num()
			 ; i++)
		{
			UUIV_RoomItem* RoomItem = CreateWidget<UUIV_RoomItem>(GetWorld(), RoomItemFactory);
			MultiPlayView->GetRoomList()->AddChild(RoomItem);

			RoomItem->SetInfo(SessionSearchData.SessionSearch->SearchResults[i]);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("세션을 검색하는 것에 실패하였습니다."));
	}
}

void UUIC_MultiPlay::OnJoinSession(FName SessionName,
                                   EOnJoinSessionCompleteResult::Type Type)
{
	UE_LOG(LogTemp, Error, TEXT("세션참가"));

	UUIV_MultiPlay* MultiPlayView = Cast<UUIV_MultiPlay>(GetView());

	FString Address;
	if (FSessionUtil::OnlineSessionInterface->GetResolvedConnectString(
		NAME_GameSession, Address))
	{
		MultiPlayView->GetOwningPlayer()->
		               ClientTravel(Address, TRAVEL_Absolute);
	}
}
