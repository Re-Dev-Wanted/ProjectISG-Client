// Fill out your copyright notice in the Description page of Project Settings.


#include "UIC_MainLobby.h"

#include "GameFramework/PlayerState.h"
#include "ProjectISG/Utils/SessionUtil.h"

void UUIC_MainLobby::CreateSession()
{
	UE_LOG(LogTemp, Warning, TEXT("세션을 생성합니다."));

	FSessionCreateData CreateData;
	CreateData.MaxPlayer = 4;
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

void UUIC_MainLobby::OnCompleteCreate(FName SessionName, bool IsCreateSuccess)
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
