// Fill out your copyright notice in the Description page of Project Settings.


#include "GranmaHouseDiary.h"

#include "Kismet/GameplayStatics.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"


// Sets default values
AGranmaHouseDiary::AGranmaHouseDiary()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGranmaHouseDiary::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
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
		LoadPlayLevelAndTeleportPlayer(Player);
	}

	// 메인레벨 이동
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

