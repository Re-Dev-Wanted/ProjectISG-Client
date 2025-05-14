// Fill out your copyright notice in the Description page of Project Settings.


#include "UIC_Loading.h"

#include "GameFramework/PlayerState.h"
#include "ProjectISG/Core/Controller/LobbyPlayerController.h"
#include "ProjectISG/Utils/SessionUtil.h"


void UUIC_Loading::CreateSession()
{
	UE_LOG(LogTemp, Warning, TEXT("세션을 생성합니다."));
	
	FSessionCreateData CreateData;
	CreateData.MaxPlayer = 4;
	FString RoomName = FString::Printf(
		TEXT("%s 님의 농장"),
		*GetPlayerController()->GetPlayerState<APlayerState>()->GetPlayerName());
	CreateData.RoomName = RoomName;
	CreateData.OnCreateSessionCompleteDelegate =
		FOnCreateSessionCompleteDelegate::CreateUObject(
			this, &ThisClass::OnCompleteCreate);
	
	FSessionUtil::CreateSession(CreateData);
}

void UUIC_Loading::OnCompleteCreate(FName SessionName, bool IsCreateSuccess)
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

void UUIC_Loading::JoinSession()
{
	ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(GetPlayerController());
	
	OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSession);

	FSessionUtil::JoinSession(GetWorld(), LobbyPlayerController->SessionSearchResult, OnJoinSessionCompleteDelegate);
}

void UUIC_Loading::OnJoinSession(FName SessionName,
	EOnJoinSessionCompleteResult::Type Type)
{
	FString Address;
	if (FSessionUtil::OnlineSessionInterface->GetResolvedConnectString(NAME_GameSession, Address))
	{
		GetPlayerController()->ClientTravel(Address, TRAVEL_Absolute);
	}
}
