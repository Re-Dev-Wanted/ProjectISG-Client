// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"

#include "ProjectISG/Core/UI/Base/Components/UIManageComponent.h"

ALobbyPlayerController::ALobbyPlayerController()
{
	UIManageComponent = CreateDefaultSubobject<UUIManageComponent>(
		"UI Manage Component");
}


void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		UIManageComponent->Initialize();
		UIManageComponent->PushWidget(EUIName::Popup_MainLobby);
	}
}

void ALobbyPlayerController::PushUI(const EUIName UIName)
{
	UIManageComponent->PushWidget(UIName);
}

void ALobbyPlayerController::PopUI()
{
	UIManageComponent->PopWidget();
}

void ALobbyPlayerController::ShowLoadingUI(bool bIsServerTravel)
{
	UIManageComponent->PopWidget();
	UIManageComponent->PushWidget(EUIName::Loading_LoadingUI);
	IsServerTravel = bIsServerTravel;
}
