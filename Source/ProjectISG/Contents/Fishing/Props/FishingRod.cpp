// Fill out your copyright notice in the Description page of Project Settings.

#include "FishingRod.h"

#include "AbilitySystemComponent.h"
#include "Bobber.h"
#include "CableComponent.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ProjectISG/Contents/Fishing/Managers/FishingManager.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/GAS/Common/Tag/ISGGameplayTag.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"

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

	Bobber->OnBite();

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

	if (bLoop)
	{
		OnStartFishing();
	}
}

bool AFishingRod::GetCanTouch() const
{
	return true;
}

FString AFishingRod::GetDisplayText() const
{
	return TEXT("올리기");
}

void AFishingRod::OnTouch(AActor* Causer)
{
	Super::OnTouch(Causer);

	// UKismetSystemLibrary::PrintString(GetWorld(),TEXT("AFishingRod::OnTouch"));

	if (AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(Causer))
	{
		FGameplayEventData EventData;
		EventData.EventTag = ISGGameplayTags::Fishing_Active_ReelInLine;
		EventData.Instigator = Player;
		EventData.Target = this;
			
		Player->GetAbilitySystemComponent()->HandleGameplayEvent(ISGGameplayTags::Fishing_Active_ReelInLine, &EventData);
	}
}

void AFishingRod::OnTouchResponse()
{
	Super::OnTouchResponse();
}

void AFishingRod::StartCasting(FVector Destination)
{
	if (!Bobber)
	{
		return;
	}

	Bobber->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Bobber->SetActorLocation(CastingStartPoint->GetComponentLocation());
	
	Bobber->SuggestProjectileVelocity(CastingStartPoint->GetComponentLocation(),Destination);
}

void AFishingRod::ReelInLine(AActor* Causer)
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
	
	Bobber->SetCollisionAndPhysicsEnabled(false);
	Bobber->AttachToComponent(PocketSocketComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	OnEventFinish(false);
}
