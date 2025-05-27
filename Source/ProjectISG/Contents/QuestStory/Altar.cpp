// Fill out your copyright notice in the Description page of Project Settings.


#include "Altar.h"

#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"


AAltar::AAltar()
{
	PrimaryActorTick.bCanEverTick = true;
	
}

void AAltar::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAltar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAltar::OnInteractive(AActor* Causer)
{
	Super::OnInteractive(Causer);

	AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(Causer);
	AMainPlayerState* PS = Player->GetPlayerState<AMainPlayerState>();
	AMainPlayerController* PC = Player->GetController<AMainPlayerController>();
	
	if (Player && PS)
	{
		if (PS->GetInventoryComponent()->HasItemInInventory(OfferingFoodId, 1))
		{
			// 공물 음식 삭제
			PS->GetInventoryComponent()->RemoveItem(OfferingFoodId, 1);
			// 퀘스트 완료
			PC->SetCustomQuestComplete(true);
			// 엔딩처리
			UE_LOG(LogTemp, Warning, TEXT("엔딩 처리"));
		}
	}
}

FString AAltar::GetInteractiveDisplayText() const
{
	return TEXT("제단에 공물 바치기");
}

bool AAltar::GetCanInteractive() const
{
	return true;
}

