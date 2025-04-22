// Fill out your copyright notice in the Description page of Project Settings.


#include "AT_Watering.h"

#include "ProjectISG/Contents/Farming/BaseCrop.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"


UAT_Watering* UAT_Watering::InitialEvent(UGameplayAbility* OwningAbility)
{
	UE_LOG(LogTemp, Warning, TEXT("123"));
	UAT_Watering* NewTask = NewAbilityTask<UAT_Watering>(OwningAbility);
	return NewTask;
}

void UAT_Watering::Activate()
{
	Super::Activate();
	player = Cast<AMainPlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
}

void UAT_Watering::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	// linetrace로 씨앗 유무 확인하기
	FVector start = player->GetActorLocation();
	FVector end = player->GetActorLocation() + player->GetActorForwardVector() * WateringDistance;
	FindSeed(start, end, FVector(100.f));
}

void UAT_Watering::FindSeed(FVector start, FVector end, FVector boxHalfSize)
{
	// FCollisionQueryParams Params;
	//
	// Params.AddIgnoredActor(GetWorld()->GetFirstPlayerController()->GetPawn());
	//
	// float ClosestDistance = TNumericLimits<float>::Max();
	//
	// TArray<FHitResult> InitialHitResults;
	// FVector InitialBoxSize = boxHalfSize;
	//
	// bool bInitialHit = GetWorld()->SweepMultiByChannel(
	// 	InitialHitResults,
	// 	start,
	// 	end,
	// 	player->GetRootComponent()->GetComponentQuat(),
	// 	ECC_Pawn,
	// 	FCollisionShape::MakeBox(InitialBoxSize),
	// 	Params);
	//
	// if (bInitialHit)
	// {
	// 	for (const FHitResult& Hit : InitialHitResults)
	// 	{
	// 		ABaseCrop* PotentialSeed = Cast<ABaseCrop>(Hit.GetActor());
	// 		if (PotentialSeed && PotentialSeed != FoundSeed)
	// 		{
	// 			float Distance = FVector::Distance(start, Hit.ImpactPoint);
	// 			{
	// 				if (Distance < ClosestDistance)
	// 				{
	// 					ClosestDistance = Distance;
	// 					FoundSeed = PotentialSeed;
	// 				}
	// 			}
	// 		}
	// 		else if (FoundSeed == PotentialSeed)
	// 		{
	// 			continue;
	// 		}
	// 		else
	// 		{
	// 			FoundSeed = nullptr;
	// 		}
	// 	}
	// }
	//
	// if (FoundSeed != nullptr)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Seed Found"));
	// }

	FindSeedDelegate.Broadcast();
}
