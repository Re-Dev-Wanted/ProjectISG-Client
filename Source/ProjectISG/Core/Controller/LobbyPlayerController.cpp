// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"

#include "ProjectISG/Core/UI/Base/Components/UIManageComponent.h"
#include "ProjectISG/Core/UI/Loading/UIC_Loading.h"

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
	UUIC_Loading* LoadingController = Cast<UUIC_Loading>(UIManageComponent->ControllerInstances[EUIName::Loading_LoadingUI]);
	if (LoadingController)
	{
		LoadingController->SetIsServerTravel(bIsServerTravel);
	}
}
