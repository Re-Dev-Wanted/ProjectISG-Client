// Fill out your copyright notice in the Description page of Project Settings.


#include "UIC_MultiPlay.h"

#include "OnlineSessionSettings.h"
#include "UIV_MultiPlay.h"
#include "Components/ScrollBox.h"
#include "ProjectISG/Core/Controller/LobbyPlayerController.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/Controller/TutorialPlayerController.h"
#include "ProjectISG/Core/UI/Popup/Lobby/RoomItem/UIV_RoomItem.h"
#include "ProjectISG/Utils/SessionUtil.h"


void UUIC_MultiPlay::SearchGameAndJoinSession()
{
	UE_LOG(LogTemp, Warning, TEXT("세션을 검색합니다."));

	SessionSearchData.SessionSearch = MakeShared<FOnlineSessionSearch>();
	SessionSearchData.OnFindSessionsCompleteDelegate =
		FOnFindSessionsCompleteDelegate::CreateUObject(
			this, &ThisClass::OnCompleteSearchAndTryJoin);
	FSessionUtil::SearchSession(SessionSearchData);
}

void UUIC_MultiPlay::FindSession()
{
	UE_LOG(LogTemp, Warning, TEXT("세션을 검색합니다."));

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
		ALobbyPlayerController* LobbyPlayerController = Cast<
			ALobbyPlayerController>(GetPlayerController());
		if (LobbyPlayerController)
		{
			for (int i = 0; i < SessionSearchData.SessionSearch->SearchResults.
			                                      Num()
			     ; i++)
			{
				UE_LOG(LogTemp, Warning, TEXT("세션을 검색하는 것에 성공하였습니다."));
				LobbyPlayerController->SessionSearchResult = SessionSearchData.
					SessionSearch->SearchResults[i];
				LobbyPlayerController->ShowLoadingUIAndCreateSession(false);
			}
		}
		else
		{
			ATutorialPlayerController* TutorialPlayerController = Cast<ATutorialPlayerController>(GetPlayerController());
			if (TutorialPlayerController)
			{
				for (int i = 0; i < SessionSearchData.SessionSearch->
				     SearchResults.Num()
				     ; i++)
				{
					UE_LOG(LogTemp, Warning, TEXT("세션을 검색하는 것에 성공하였습니다."));
					TutorialPlayerController->SessionSearchResult = SessionSearchData.
					                          SessionSearch->SearchResults[i];
					TutorialPlayerController->StartScene6(false);
				}
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("세션을 검색하는 것에 실패하였습니다."));
	}
}

void UUIC_MultiPlay::OnCompleteSearch(bool IsSearchSuccess)
{
	if (IsSearchSuccess)
	{
		UUIV_MultiPlay* MultiPlayView = Cast<UUIV_MultiPlay>(GetView());
		MultiPlayView->GetRoomList()->ClearChildren();
		for (int i = 0; i < SessionSearchData.SessionSearch->SearchResults.Num()
		     ; i++)
		{
			UE_LOG(LogTemp, Warning, TEXT("세션을 검색하는 것에 성공하였습니다. %d"), i);
			UUIV_RoomItem* RoomItem = CreateWidget<UUIV_RoomItem>(
				GetWorld(), RoomItemFactory);
			RoomItem->
				SetInfo(SessionSearchData.SessionSearch->SearchResults[i]);
			MultiPlayView->GetRoomList()->AddChild(RoomItem);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("세션을 검색하는 것에 실패하였습니다."));
	}
}
