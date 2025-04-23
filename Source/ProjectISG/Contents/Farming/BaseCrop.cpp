// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCrop.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/InteractionComponent.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/GAS/Common/Tag/ISGGameplayTag.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"
#include "ProjectISG/Systems/Time/TimeManager.h"


// Sets default values
ABaseCrop::ABaseCrop()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	Super::SetReplicateMovement(true);


	Root = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));

	InteractionPos = CreateDefaultSubobject<USceneComponent>(TEXT("InteractionPos"));
	InteractionPos->SetupAttachment(Root);


	CanInteractive = true;
}

// Called when the game starts or when spawned
void ABaseCrop::BeginPlay()
{
	Super::BeginPlay();

	TimeManager = Cast<ATimeManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(),
		                                  ATimeManager::StaticClass()));
	if (!TimeManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("타임 매니저가 없습니다"));
		return;
	}

	DisplayText = TEXT("물주기");
	CropRemainGrowTime = CropTotalGrowDay * 24;

	TimeManager->AddSleepTimeToCrop.AddDynamic(this, &ThisClass::UpdateGrowTimeBySleep);
}

void ABaseCrop::GetLifetimeReplicatedProps(
	TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseCrop, bIsMature);
	DOREPLIFETIME(ABaseCrop, bIsGetWater);
}

// Called every frame
void ABaseCrop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsGetWater && !bIsMature)
	{
		CheckGrowTime();
	}

	if (bIsGetWater)
	{
		CheckWaterDurationTime();
	}
}

void ABaseCrop::CheckGrowTime()
{
	if (bIsMature)
	{
		return;
	}
	if (bIsGetWater == false)
	{
		return;
	}

	// 시간단위로 바꿔서 현재 월드의 시간을 구한다.
	int32 CropCurrentGrowDay = TimeManager->GetDay() - CropStartGrowDay;
	int32 CropCurrentGrowTime = (CropCurrentGrowDay * 24) + (TimeManager->
		GetHour()) + (TimeManager->GetMinute() / 60) + (TimeManager->GetSecond()
		/ 3600);


	// 현재 시간과 씨앗을 심은 시간을 빼서 현재 지난 시간을 구한다.
	CropGrowTime = CropCurrentGrowTime - CropStartGrowTime;
	UE_LOG(LogTemp, Warning, TEXT("CropGrowTime : %d"), CropGrowTime);


	// 농작물의 성장 시간 변수와 비교한다
	if (CropGrowTime >= CropRemainGrowTime)
	{
		UE_LOG(LogTemp, Warning, TEXT("다 자랐다"));
		bIsMature = true;
		DisplayText = TEXT("수확하기");
		SetActorScale3D(FVector(2.0f));
	}
}

void ABaseCrop::CheckWaterDurationTime()
{
	// 물을 준 후 부터 작물이 자라기 시작하기에 물 준후 지난시간과 작물의 총 성장시간은 동일하다.

	if (CropGrowTime >= WaterDuration)
	{
		bIsGetWater = false;
		CanInteractive = true;
		CropRemainGrowTime -= CropGrowTime;
		UE_LOG(LogTemp, Warning, TEXT("cropremaingrowtime : %d"),
		       CropRemainGrowTime);

		CropGrowTime = 0.f;
	}
}

void ABaseCrop::OnInteractive(AActor* Causer)
{
	IInteractionInterface::OnInteractive(Causer);

	if (CanInteractive == false) return;

	bIsGetWater = true;
	CanInteractive = false;

	// 물주기 시작
	CropStartGrowDay = TimeManager->GetDay();
	CropStartGrowTime = (TimeManager->GetHour()) + (TimeManager->GetMinute() /
		60) + (TimeManager->GetSecond() / 3600);

	Causer->SetActorLocation(InteractionPos->GetComponentLocation());

	FGameplayTagContainer ActivateTag;
	if (bIsMature)
	{
		ActivateTag.AddTag(ISGGameplayTags::Farming_Active_Harvest);
		const AMainPlayerCharacter* player = Cast<AMainPlayerCharacter>(Causer);
		if (player)
		{
			const AMainPlayerState* ps = Cast<AMainPlayerState>(player->GetPlayerState());

			if (ps)
			{
				ps->GetInventoryComponent()->AddItem(UItemManager::GetInitialItemMetaDataById(CropId));
			}
			player->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(
				ActivateTag);
		}
		Destroy();
	}
	else
	{
		ActivateTag.AddTag(ISGGameplayTags::Farming_Active_Watering);
		const AMainPlayerCharacter* player = Cast<AMainPlayerCharacter>(Causer);
		if (player)
		{
			player->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(
				ActivateTag);
		}
	}
}

void ABaseCrop::UpdateGrowTimeBySleep()
{
	if (bIsGetWater == false) return;

	// 총 수면 시간 체크 (24시간에서 자기 시작한 시간을 뺀 후 기상시간인 6시를 더해준다)
	int32 TotalSleepTime = (24 - TimeManager->GetTimeStoppedTime()) + 6;
	// 작물의 총 성장시간 체크

	if (CropRemainGrowTime > TotalSleepTime)
	{
		// water duration과 물 준후의 지난 시간 체크
		int32 RemainWaterDuration = WaterDuration - CropGrowTime;
		if (RemainWaterDuration <= TotalSleepTime)
		{
			bIsGetWater = false;
			CanInteractive = true;
			CropRemainGrowTime -= WaterDuration;
			UE_LOG(LogTemp, Warning, TEXT("CropRemainGrowTime : %d"), CropRemainGrowTime);
			CropGrowTime = 0.f;
		}
		else
		{
			CropGrowTime += TotalSleepTime;
		}
	}
	else
	{
		bIsMature = true;
	}
}
