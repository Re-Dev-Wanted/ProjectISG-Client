// Fill out your copyright notice in the Description page of Project Settings.


#include "UIV_MultiPlay.h"

#include "UIC_MultiPlay.h"
#include "Components/Button.h"


void UUIV_MultiPlay::NativeConstruct()
{
	Super::NativeConstruct();

	CreateRoom->OnClicked.AddDynamic(
		this, &ThisClass::OnClickedCreateRoomButton);
	JoinSession->OnClicked.AddDynamic(
		this, &ThisClass::OnClickedJoinSessionButton);

	UUIC_MultiPlay* MultiPlayController = Cast<UUIC_MultiPlay>(GetController());
	MultiPlayController->FindSession();
}

void UUIV_MultiPlay::OnClickedCreateRoomButton()
{
	UUIC_MultiPlay* MultiPlayUIController = Cast<UUIC_MultiPlay>(
		GetController());
	MultiPlayUIController->CreateSession();
}

void UUIV_MultiPlay::OnClickedJoinSessionButton()
{
	UUIC_MultiPlay* MultiPlayUIController = Cast<UUIC_MultiPlay>(
		GetController());
	MultiPlayUIController->SearchGameAndJoinSession();
}