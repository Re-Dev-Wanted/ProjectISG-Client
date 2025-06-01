// Fill out your copyright notice in the Description page of Project Settings.


#include "UIC_MainLobby.h"

#include "LevelSequencePlayer.h"
#include "UIM_MainLobby.h"
#include "UIV_MainLobby.h"
#include "ProjectISG/Core/ISGGameInstance.h"
#include "ProjectISG/Core/Controller/LobbyPlayerController.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"


void UUIC_MainLobby::PlayLobbyLS()
{
	UUIM_MainLobby* MainLobbyModel = Cast<UUIM_MainLobby>(GetModel());

	ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(GetPlayerController());
	LobbyPlayerController->PopUI();
	FMovieSceneSequencePlaybackSettings PlaybackSettings;
	PlaybackSettings.bAutoPlay = true;
	ULevelSequencePlayer* LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), MainLobbyModel->LobbyLS, PlaybackSettings, MainLobbyModel->LevelSequenceActor);
	LevelSequencePlayer->OnFinished.AddDynamic(this, &ThisClass::OnFinishLS);
	LevelSequencePlayer->Play();
}

void UUIC_MainLobby::OnFinishLS()
{
	UISGGameInstance* ISGGameInstance = Cast<UISGGameInstance>(GetView()->GetGameInstance());
	if (ISGGameInstance)
	{
		ISGGameInstance->CreateGameSessionId();
	}
}
