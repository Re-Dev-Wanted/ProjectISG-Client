// Fill out your copyright notice in the Description page of Project Settings.

#include "FishingRod.h"

#include "AbilitySystemComponent.h"
#include "Bobber.h"
#include "CableComponent.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Misc/MapErrors.h"
#include "ProjectISG/Contents/Fishing/Managers/FishingManager.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/GAS/Common/Tag/ISGGameplayTag.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"
#include "ProjectISG/Systems/Logging/LoggingEnum.h"
#include "ProjectISG/Systems/Logging/LoggingStruct.h"
#include "ProjectISG/Systems/Logging/LoggingSubSystem.h"

AFishingRod::AFishingRod()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComp->SetRelativeLocation(FVector(0.f, 0.f, 5.f));
	MeshComp->SetRelativeScale3D(FVector::OneVector * 0.3f);
	MeshComp->SetRelativeRotation(FRotator(0.f, -90.f, 180.f));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SocketComp = CreateDefaultSubobject<USceneComponent>(TEXT("SocketComp"));
	SocketComp->SetupAttachment(MeshComp, TEXT("ThreadSocket"));

	PocketSocketComp = CreateDefaultSubobject<USceneComponent>(TEXT("PocketSocketComp"));
	PocketSocketComp->SetupAttachment(MeshComp, TEXT("PocketSocket"));

	CastingStartPoint = CreateDefaultSubobject<USceneComponent>(TEXT("CastingStartPoint"));
	CastingStartPoint->SetupAttachment(MeshComp);

	Thread = CreateDefaultSubobject<UCableComponent>(TEXT("ThreadComp"));
	Thread->SetupAttachment(SocketComp);
	Thread->bAttachEnd = true;
	Thread->SetAttachEndTo(this, TEXT("PocketSocketComp"));
	Thread->EndLocation = FVector::ZeroVector;
}


void AFishingRod::BeginPlay()
{
	Super::BeginPlay();

	if (!Bobber)
	{
		Bobber = GetWorld()->SpawnActor<ABobber>(BobberFactory);
		Bobber->OnEnterWater.BindDynamic(this, &AFishingRod::OnStartFishing);
		Bobber->SetCollisionAndPhysicsEnabled(false);
		Bobber->AttachToComponent(PocketSocketComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

		Thread->SetAttachEndTo(Bobber, TEXT("LineAttachPoint"));
	}

	TimerHandles.Add(FTimerHandle());
	TimerHandles.Add(FTimerHandle());
	TimerHandles.Add(FTimerHandle());
}

void AFishingRod::Destroyed()
{
	Super::Destroyed();

	if (Bobber)
	{
		Bobber->Destroy();
	}
}

void AFishingRod::OnStartFishing()
{
	//TODO: 물고기를 여기에서 정하지만 옮길 수도 있음
	FishData = UFishingManager::GetRandomData();
	float WaitTime = FishData.GetWaitTime();
	
	GetWorld()->
	GetTimerManager()
	.SetTimer
	(
		TimerHandles[0],
		this,
		&AFishingRod::OnEventBite,
		WaitTime,
		false
	);
}

void AFishingRod::OnEventBite()
{
	if (!Bobber)
	{
		return;
	}


	Bobber->OnBite(FishData.GetMesh());

	GetWorld()->
	GetTimerManager()
	.SetTimer
	(
		TimerHandles[1],
		this,
		&AFishingRod::OnEventRealBite,
		BitingCheckDelayTime,
		false
	);
	
}

void AFishingRod::OnEventRealBite()
{
	IsBiteFish = true;

	BiteLogging();

	TWeakObjectPtr<AFishingRod> WeakThis = this;
	
	GetWorld()->
	GetTimerManager()
	.SetTimer
	(
		TimerHandles[2],
		[WeakThis] ()
		{
			if (WeakThis.IsValid())
			{
				WeakThis->OnEventFinish(true);
			}
		},
		BitingTime,
		false
	);
}

void AFishingRod::OnEventFinish(bool bLoop)
{
	IsBiteFish = false;
	FishData = FFishData();

	if (Bobber)
	{
		Bobber->RemoveFish();
	}

	if (bLoop)
	{
		OnStartFishing();
	}
}

void AFishingRod::BiteLogging()
{
	FDiaryLogParams LogParams;
	LogParams.Location = TEXT("연못");
	LogParams.ActionType = ELoggingActionType::FISHING;
	LogParams.ActionName = ELoggingActionName::hook_bite;

	GetWorld()->GetGameInstance()->GetSubsystem<ULoggingSubSystem>()->
				LoggingData(LogParams);

	GetWorld()->GetGameInstance()->GetSubsystem<ULoggingSubSystem>()->Flush();
}

void AFishingRod::FinishLogging(bool bSuccess)
{
	FDiaryLogParams LogParams;
	LogParams.Location = TEXT("연못");
	LogParams.ActionType = ELoggingActionType::FISHING;
	LogParams.ActionName = ELoggingActionName::finish_fishing;
	LogParams.Detail = bSuccess? TEXT("물고기를 낚았다!") : TEXT("물고기를 낚지 못했다.");

	GetWorld()->GetGameInstance()->GetSubsystem<ULoggingSubSystem>()->
				LoggingData(LogParams);

	GetWorld()->GetGameInstance()->GetSubsystem<ULoggingSubSystem>()->Flush();
}

// bool AFishingRod::GetCanTouch() const
// {
// 	return false;
// }
//
// FString AFishingRod::GetTouchDisplayText() const
// {
// 	return TEXT("올리기");
// }
//
// void AFishingRod::OnTouch(AActor* Causer)
// {
// 	Super::OnTouch(Causer);
//
// 	// UKismetSystemLibrary::PrintString(GetWorld(),TEXT("AFishingRod::OnTouch"));
//
// 	if (AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(Causer))
// 	{
// 		FGameplayEventData EventData;
// 		EventData.EventTag = ISGGameplayTags::Fishing_Active_ReelInLine;
// 		EventData.Instigator = Player;
// 		EventData.Target = this;
// 			
// 		Player->GetAbilitySystemComponent()->HandleGameplayEvent(ISGGameplayTags::Fishing_Active_ReelInLine, &EventData);
// 	}
// }
//
// void AFishingRod::OnTouchResponse(AActor* Causer)
// {
// 	Super::OnTouchResponse(Causer);
// }

void AFishingRod::StartCasting(AActor* Causer, FVector Destination)
{
	if (!Bobber)
	{
		return;
	}

	Bobber->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Bobber->SetActorLocation(CastingStartPoint->GetComponentLocation());

	FVector EndLocation = Destination + Causer->GetActorForwardVector() * CastingDistance;
	
	Bobber->SuggestProjectileVelocity(Causer->GetActorLocation(), EndLocation);
}

void AFishingRod::ReelInLine()
{
	if (!Bobber)
	{
		return;
	}

	for (FTimerHandle Handle : TimerHandles)
	{
		GetWorld()->GetTimerManager().ClearTimer(Handle);
		Handle.Invalidate();
	}
	
	Bobber->SetCollisionAndPhysicsEnabled(false);
	Bobber->AttachToComponent(PocketSocketComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void AFishingRod::OnEndReelInLine(AActor* Causer)
{
	if (IsBiteFish && FishData.IsValid())
	{
		// UKismetSystemLibrary::PrintString(GetWorld(), TEXT("물고기 잡음!"));
		
		const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(Causer);
		
		if (Player)
		{
			AMainPlayerState* PlayerState = Cast<AMainPlayerState>(Player->GetController()->PlayerState);

			if (PlayerState)
			{
				FItemMetaInfo FishMetaInfo = UItemManager::GetInitialItemMetaDataById(FishData.GetItemId());
				PlayerState->GetInventoryComponent()->AddItem(FishMetaInfo);
			}
		}

	}

	FinishLogging(IsBiteFish);

	OnEventFinish(false);
}
