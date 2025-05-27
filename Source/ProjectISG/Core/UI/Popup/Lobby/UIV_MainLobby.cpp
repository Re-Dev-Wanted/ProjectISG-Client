// Fill out your copyright notice in the Description page of Project Settings.


#include "UIV_MainLobby.h"

#include "UIC_MainLobby.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ProjectISG/Core/ISGGameInstance.h"
#include "ProjectISG/Core/Controller/LobbyPlayerController.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"


void UUIV_MainLobby::NativeConstruct()
{
	Super::NativeConstruct();

	GameStart->OnClicked.AddDynamic(
		this, &ThisClass::OnClickedGameStartButton);

	Multiplay->OnClicked.AddDynamic(
		this, &ThisClass::OnClickedMultiplayButton);

	Exit->OnClicked.AddDynamic(this, &ThisClass::OnClickedExitButton);
}

void UUIV_MainLobby::OnClickedGameStartButton()
{
	UISGGameInstance* ISGGameInstance = Cast<UISGGameInstance>(GetGameInstance());
	if (ISGGameInstance)
	{
		ISGGameInstance->CreateGameSessionId();
	}
}

void UUIV_MainLobby::OnClickedMultiplayButton()
{
	UUIC_MainLobby* MainLobbyController = Cast<UUIC_MainLobby>(GetController());
	ALobbyPlayerController* LobbyPlayerController = Cast<
		ALobbyPlayerController>(MainLobbyController->GetPlayerController());
	LobbyPlayerController->PushUI(EUIName::Popup_MultiPlay);
}

void UUIV_MainLobby::OnClickedExitButton()
{
	UKismetSystemLibrary::QuitGame(this, GetWorld()->GetFirstPlayerController(),
	                               EQuitPreference::Quit, false);
}
