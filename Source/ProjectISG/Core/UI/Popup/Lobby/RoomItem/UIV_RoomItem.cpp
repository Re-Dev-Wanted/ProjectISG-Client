// Fill out your copyright notice in the Description page of Project Settings.


#include "UIV_RoomItem.h"

#include "UIC_RoomItem.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "ProjectISG/Core/Controller/LobbyPlayerController.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/Controller/TutorialPlayerController.h"
#include "ProjectISG/Utils/SessionUtil.h"

void UUIV_RoomItem::NativeConstruct()
{
	Super::NativeConstruct();

	JoinButton->OnClicked.AddDynamic(
		this, &ThisClass::UUIV_RoomItem::OnClickedJoinButton);
}

void UUIV_RoomItem::OnClickedJoinButton()
{
	UUIC_RoomItem* RoomItemController = Cast<UUIC_RoomItem>(GetController());
	ALobbyPlayerController* LobbyPlayerController = Cast<
		ALobbyPlayerController>(RoomItemController->GetPlayerController());

	if (LobbyPlayerController)
	{
		LobbyPlayerController->SessionSearchResult = this->SessionSearchResult;
		LobbyPlayerController->ShowLoadingUIAndCreateSession(false);
	}
	else
	{
		ATutorialPlayerController* TutorialPlayerController = Cast<
			ATutorialPlayerController>(
			RoomItemController->GetPlayerController());

		if (TutorialPlayerController)
		{
			TutorialPlayerController->SessionSearchResult = this->
				SessionSearchResult;
			TutorialPlayerController->StartScene6(false);
		}
	}
}

void UUIV_RoomItem::SetInfo(const FOnlineSessionSearchResult& SearchResult)
{
	SessionSearchResult = SearchResult;

	FString RoomName;
	SearchResult.Session.SessionSettings.Get(TEXT("RoomName"), RoomName);

	RoomNameText->
		SetText(FText::FromString(FSessionUtil::DecodeData(RoomName)));

	const uint8 MaxPlayerCount = SearchResult.Session.SessionSettings.
	                                          NumPublicConnections;
	const uint8 RemainPlayerCount = SearchResult.Session.
	                                             NumOpenPublicConnections;

	CurrentPlayerText->SetText(
		FText::AsNumber(MaxPlayerCount - RemainPlayerCount));
	MaxPlayerText->SetText(FText::AsNumber(MaxPlayerCount));
}
