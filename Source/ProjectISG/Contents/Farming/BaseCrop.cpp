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
#include "ProjectISG/Utils/EnumUtil.h"


ABaseCrop::ABaseCrop()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;
	SetReplicatingMovement(true);

	Root = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Root->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
	Mesh->SetRenderCustomDepth(true);

	InteractionPos = CreateDefaultSubobject<USceneComponent>(
		TEXT("InteractionPos"));
	InteractionPos->SetupAttachment(Root);
}

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

	CropRemainGrowTime = CropTotalGrowDay * 24;

	TimeManager->AddSleepTimeToCrop.AddDynamic(
		this, &ThisClass::UpdateGrowTimeBySleep);
}

void ABaseCrop::GetLifetimeReplicatedProps(
	TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseCrop, bIsGetWater);
	DOREPLIFETIME(ABaseCrop, CurrentState);
}

void ABaseCrop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HasAuthority() == false)
	{
		return;
	}

	if (bIsGetWater && CurrentState == ECropState::Sprout)
	{
		CheckGrowTime();
	}

	if (bIsGetWater)
	{
		CheckWaterDurationTime();
	}
}

bool ABaseCrop::GetCanInteractive() const
{
	return CurrentState == ECropState::Mature;
}

FString ABaseCrop::GetInteractiveDisplayText() const
{
	return TEXT("수확하기");
}

void ABaseCrop::CheckGrowTime()
{
	if (CurrentState == ECropState::Mature || !bIsGetWater)
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

	// 농작물의 성장 시간 변수와 비교한다
	if (CropGrowTime >= CropRemainGrowTime)
	{
		UE_LOG(LogTemp, Warning, TEXT("다 자랐다"));
		if (HasAuthority())
		{
			Server_CropIsMature();
		}
	}
}

void ABaseCrop::CheckWaterDurationTime()
{
	// 물을 준 후 부터 작물이 자라기 시작하기에 물 준후 지난시간과 작물의 총 성장시간은 동일하다.

	if (CropGrowTime >= WaterDuration)
	{
		bIsGetWater = false;

		CropRemainGrowTime -= CropGrowTime;
		CropGrowTime = 0.f;
	}
}

void ABaseCrop::OnInteractive(AActor* Causer)
{
	IInteractionInterface::OnInteractive(Causer);

	if (CurrentState != ECropState::Mature)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("작물 상호작용 함수 실행 , 로컬롤 : %s"),
	       *FEnumUtil::GetClassEnumKeyAsString(GetLocalRole()));

	Causer->SetActorLocation(InteractionPos->GetComponentLocation());
	Causer->SetActorRotation(InteractionPos->GetComponentRotation());

	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(Causer);
	FGameplayTagContainer ActivateTag;
	ActivateTag.AddTag(ISGGameplayTags::Farming_Active_Harvest);
	Player->GetPlayerState<AMainPlayerState>()->GetInventoryComponent()->
	        AddItem(UItemManager::GetInitialItemMetaDataById(CropId));

	Player->GetInteractionComponent()->Server_OnInteractiveResponse(Causer);
	Player->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(
		ActivateTag);
}

void ABaseCrop::OnInteractiveResponse(AActor* Causer)
{
	IInteractionInterface::OnInteractiveResponse(Causer);

	Destroy();
}

void ABaseCrop::UpdateGrowTimeBySleep()
{
	if (bIsGetWater == false)
	{
		return;
	}

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
			CropRemainGrowTime -= WaterDuration;
			CropGrowTime = 0.f;
		}
		else
		{
			CropGrowTime += TotalSleepTime;
		}
	}
	else
	{
		if (HasAuthority())
		{
			Server_CropIsMature();
		}
	}
}

void ABaseCrop::OnRep_UpdateState()
{
	if (CurrentState == ECropState::Mature)
	{
		Root->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

void ABaseCrop::Server_CropIsMature_Implementation()
{
	SetCurrentState(ECropState::Mature);
	NetMulticast_ChangeCropMeshToMature();
}

void ABaseCrop::NetMulticast_ChangeCropMeshToMature_Implementation()
{
	SetActorScale3D(FVector(2.0f));
	OnDryField.Broadcast();
}

void ABaseCrop::CropIsGetWater()
{
	if (bIsGetWater)
	{
		return;
	}

	bIsGetWater = true;
	CropStartGrowDay = TimeManager->GetDay();
	CropStartGrowTime = (TimeManager->GetHour()) + (TimeManager->
		GetMinute() /
		60) + (TimeManager->GetSecond() / 3600);
}

void ABaseCrop::SetCurrentState(ECropState State)
{
	CurrentState = State;

	if (CurrentState == ECropState::Mature)
	{
		Root->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}
