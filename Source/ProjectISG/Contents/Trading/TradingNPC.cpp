// Fill out your copyright notice in the Description page of Project Settings.


#include "TradingNPC.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "Components/BoxComponent.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/GAS/Common/Tag/ISGGameplayTag.h"


class AMainPlayerCharacter;

ATradingNPC::ATradingNPC()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	Super::SetReplicateMovement(true);

	Root = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));

	InteractionPos = CreateDefaultSubobject<USceneComponent>(
		TEXT("InteractionPos"));
	InteractionPos->SetupAttachment(Root);

	CanInteractive = true;
}

void ATradingNPC::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATradingNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATradingNPC::OnInteractive(AActor* Causer)
{
	IInteractionInterface::OnInteractive(Causer);
	
	Causer->SetActorLocation(InteractionPos->GetComponentLocation());
	Causer->SetActorRotation(InteractionPos->GetComponentRotation());

	FGameplayTagContainer ActivateTag;
	ActivateTag.AddTag(ISGGameplayTags::Trading_Active_OpenTradingUI);
	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(Causer);
	if (Player)
	{
		Player->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(
			ActivateTag);
	}
}

bool ATradingNPC::GetCanInteractive() const
{
	return CanInteractive;
}

FString ATradingNPC::GetDisplayText() const
{
	return TEXT("거래하기");
}
