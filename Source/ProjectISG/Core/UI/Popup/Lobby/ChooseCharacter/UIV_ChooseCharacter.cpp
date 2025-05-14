// Fill out your copyright notice in the Description page of Project Settings.


#include "UIV_ChooseCharacter.h"

#include "Components/Button.h"

void UUIV_ChooseCharacter::NativeConstruct()
{
	Super::NativeConstruct();

	GameStart->OnClicked.AddDynamic(this, &ThisClass::OnClickedGameStart);
}

void UUIV_ChooseCharacter::OnClickedGameStart()
{
	GetWorld()->ServerTravel(
		FString("/Game/Maps/MainPlayLevel/MainPlayLevel?listen"));
}
