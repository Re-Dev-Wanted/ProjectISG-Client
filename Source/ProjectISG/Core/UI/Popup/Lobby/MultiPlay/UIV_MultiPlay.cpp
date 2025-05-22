// Fill out your copyright notice in the Description page of Project Settings.


#include "UIV_MultiPlay.h"

#include "UIC_MultiPlay.h"
#include "Components/Button.h"
#include "ProjectISG/Core/Controller/LobbyPlayerController.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"


class ALobbyPlayerController;

void UUIV_MultiPlay::NativeConstruct()
{
	Super::NativeConstruct();

	CreateRoom->OnClicked.AddDynamic(
		this, &ThisClass::OnClickedCreateRoomButton);
	JoinSession->OnClicked.AddDynamic(
		this, &ThisClass::OnClickedJoinSessionButton);
	FindRoom->OnClicked.AddDynamic(this, &ThisClass::OnClickedFindRoomButton);

	UUIC_MultiPlay* MultiPlayController = Cast<UUIC_MultiPlay>(GetController());
	MultiPlayController->FindSession();
}

void UUIV_MultiPlay::OnClickedCreateRoomButton()
{
	UUIC_MultiPlay* MultiPlayUIController = Cast<UUIC_MultiPlay>(
		GetController());
	ALobbyPlayerController* LobbyPlayerController = Cast<
		ALobbyPlayerController>(MultiPlayUIController->GetPlayerController());

	if (LobbyPlayerController)
	{
		LobbyPlayerController->ShowLoadingUIAndCreateSession(true);
		return;
	}

	AMainPlayerController* PC = Cast<AMainPlayerController>(
		MultiPlayUIController->GetPlayerController());
	if (PC)
	{
		PC->ShowLoadingUIAndCreateSession(true);
	}
}

void UUIV_MultiPlay::OnClickedJoinSessionButton()
{
	UUIC_MultiPlay* MultiPlayUIController = Cast<UUIC_MultiPlay>(
		GetController());
	MultiPlayUIController->SearchGameAndJoinSession();
}

void UUIV_MultiPlay::OnClickedFindRoomButton()
{
	UUIC_MultiPlay* MultiPlayUIController = Cast<UUIC_MultiPlay>(
		GetController());
	MultiPlayUIController->FindSession();
}
