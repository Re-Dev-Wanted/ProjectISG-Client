// Fill out your copyright notice in the Description page of Project Settings.


#include "GranmaHouseDiary.h"

#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/UI/Base/Components/UIManageComponent.h"


AGranmaHouseDiary::AGranmaHouseDiary()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGranmaHouseDiary::BeginPlay()
{
	Super::BeginPlay();
}

void AGranmaHouseDiary::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGranmaHouseDiary::OnInteractive(AActor* Causer)
{
	Super::OnInteractive(Causer);

	AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(Causer);
	if (Player)
	{
		AMainPlayerController* PC = Cast<AMainPlayerController>(
			Player->GetController());
		if (PC)
		{
			PC->PushUI(EUIName::Popup_MultiPlay);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("메인레벨 이동"));
}

FString AGranmaHouseDiary::GetInteractiveDisplayText() const
{
	return TEXT("일기장 펼치기");
}

bool AGranmaHouseDiary::GetCanInteractive() const
{
	return true;
}
