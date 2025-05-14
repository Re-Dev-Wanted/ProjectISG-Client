// Fill out your copyright notice in the Description page of Project Settings.


#include "UIV_RoomItem.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "ProjectISG/Utils/SessionUtil.h"

void UUIV_RoomItem::NativeConstruct()
{
	Super::NativeConstruct();

	JoinButton->OnClicked.AddDynamic(
		this, &ThisClass::UUIV_RoomItem::OnClickedJoinButton);

	OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSession);
}

void UUIV_RoomItem::OnClickedJoinButton()
{
	FSessionUtil::JoinSession(GetWorld(), SessionSearchResult, OnJoinSessionCompleteDelegate);
}

void UUIV_RoomItem::SetInfo(const FOnlineSessionSearchResult& SearchResult)
{
	SessionSearchResult = SearchResult;
	
	FString RoomName;
	SearchResult.Session.SessionSettings.Get(TEXT("RoomName"), RoomName);

	RoomNameText->SetText(FText::FromString(FSessionUtil::DecodeData(RoomName)));
	
	const uint8 MaxPlayerCount = SearchResult.Session.SessionSettings.NumPublicConnections;
	const uint8 RemainPlayerCount = SearchResult.Session.NumOpenPublicConnections;

	CurrentPlayerText->SetText(FText::FromString(FString::FromInt(MaxPlayerCount - RemainPlayerCount)));
	MaxPlayerText->SetText(FText::FromString(FString::FromInt(MaxPlayerCount)));
}

void UUIV_RoomItem::OnJoinSession(FName SessionName, EOnJoinSessionCompleteResult::Type Type)
{
	FString Address;
	if (FSessionUtil::OnlineSessionInterface->GetResolvedConnectString(NAME_GameSession, Address))
	{
		GetOwningPlayer()->ClientTravel(Address, TRAVEL_Absolute);
	}
}
