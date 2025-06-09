// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCrop.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/FarmingManager.h"
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

	ConstructorHelpers::FObjectFinder<UMaterial> tempMat (TEXT("'/Game/Contents/Farming/Props/M_CropCanInteract.M_CropCanInteract'"));
	if (tempMat.Succeeded())
	{
		OverlayMaterial = tempMat.Object;
	}
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

	CropTotalGrowTime = CropTotalGrowDay * 24;
	CropBecomeSprout = CropTotalGrowTime / 4;
	CropBecomeStem = CropTotalGrowTime / 2;

	TimeManager->AddSleepTimeToCrop.AddDynamic(
		this, &ThisClass::UpdateGrowTimeBySleep);

	TWeakObjectPtr<ThisClass> WeakThis = this;
	OnDryField.AddLambda
	(
		[WeakThis]()
		{
			if (WeakThis.IsValid())
			{
				WeakThis.Get()->SetOverlayInteractMaterial(true);
			}
		}
	);
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

	// 남은 성장시간을 통해 현재 작물의 상태를 확인한다.
	CheckStateByRemainGrowTime();
	if (bIsGetWater && CurrentState != ECropState::Mature)
	{
		CheckGrowTime();
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
}

void ABaseCrop::CheckStateByRemainGrowTime()
{
	int32 TotalEffectiveGrowTime = 0;

	if (bIsGetWater)
	{
		TotalEffectiveGrowTime = CropGrowTime + CropGrowTimeSum;
	}
	else
	{
		TotalEffectiveGrowTime = CropGrowTimeSum;
	}

	if (TotalEffectiveGrowTime >= CropTotalGrowTime) // 다 자란 상태 체크
	{
		if (CurrentState != ECropState::Mature)
		{
			SetCurrentState(ECropState::Mature);
		}
	}
	else if (TotalEffectiveGrowTime >= CropBecomeStem) // 줄기 상태 체크
	{
		if (CurrentState != ECropState::Stem)
		{
			SetCurrentState(ECropState::Stem);
		}
	}
	else if (TotalEffectiveGrowTime >= CropBecomeSprout) // 새싹 상태 체크
	{
		if (CurrentState != ECropState::Sprout)
		{
			SetCurrentState(ECropState::Sprout);
		}
	}
}

void ABaseCrop::CheckWaterDurationTime()
{
	// 물을 준 후 부터 작물이 자라기 시작하기에 물 준후 지난시간과 작물의 총 성장시간은 동일하다.
	if (CropGrowTime >= WaterDuration)
	{
		bIsGetWater = false;
		CropGrowTime = 0;
		CropGrowTimeSum += WaterDuration;
		Server_FieldIsDried();
	}
}

EItemGrade ABaseCrop::SetItemGrade()
{
	int32 RandomNum = FMath::RandRange(0, 100);
	UE_LOG(LogTemp, Warning, TEXT("랜덤 숫자 : %d"), RandomNum);
	if (RandomNum > 98)
	{
		return EItemGrade::Unique;
	}
	else if (RandomNum > 90)
	{
		return EItemGrade::Rare;
	}
	else if (RandomNum > 80)
	{
		return EItemGrade::Uncommon;
	}
	else
	{
		return EItemGrade::Common;
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

	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(Causer);
	FGameplayTagContainer ActivateTag;
	ActivateTag.AddTag(ISGGameplayTags::Farming_Active_Harvest);

	for (int i = 0; i < UFarmingManager::GetDataByCropId(MatureFarmingObjectId).
	     GetYield(); i++)
	{
		FItemMetaInfo CropInfo = UItemManager::GetInitialItemMetaDataById(
			UFarmingManager::GetDataByCropId(MatureFarmingObjectId).
			GetItemId());
		CropInfo.SetMetaDataValue(EMetaDataKey::ItemGrade,
		                          FEnumUtil::GetClassEnumKeyAsString(
			                          SetItemGrade()));
		Player->GetPlayerState<AMainPlayerState>()->GetInventoryComponent()->
		        AddItem(CropInfo);
	}

	Player->GetInteractionComponent()->Server_OnInteractiveResponse(Causer);
	Player->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(
		ActivateTag);
}

void ABaseCrop::OnInteractiveResponse(AActor* Causer)
{
	IInteractionInterface::OnInteractiveResponse(Causer);

	HarvestCrop.Broadcast();
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

	if (CropTotalGrowTime > TotalSleepTime)
	{
		// water duration과 물 준후의 지난 시간 체크
		int32 RemainWaterDuration = WaterDuration - CropGrowTime;
		if (RemainWaterDuration <= TotalSleepTime)
		{
			bIsGetWater = false;
			CropGrowTime = 0;
			CropGrowTimeSum += WaterDuration;
			NetMulticast_FieldIsDried();
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
	OnDryField.Broadcast();
}

void ABaseCrop::Server_FieldIsDried_Implementation()
{
	NetMulticast_FieldIsDried();
}

void ABaseCrop::NetMulticast_FieldIsDried_Implementation()
{
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
	if (State == CurrentState)
	{
		return;
	}
	ChangeCurrentCropState(State);

	if (CurrentState == ECropState::Mature)
	{
		Root->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

void ABaseCrop::ChangeCurrentCropState(ECropState State)
{
	CurrentState = State;
	NetMulticast_ChangeCurrentCropState(State);
}

void ABaseCrop::NetMulticast_ChangeCurrentCropState_Implementation(
	ECropState State)
{
	switch (State)
	{
	case ECropState::Seedling:
		{
			Mesh->SetRelativeScale3D(
				UFarmingManager::GetDataByCropId(1).GetScale());
			Mesh->SetStaticMesh(
				UFarmingManager::GetDataByCropId(1).GetStaticMesh());
			Mesh->SetMaterial(
				0, UFarmingManager::GetDataByCropId(1).GetMeshMaterial());
			SetOverlayInteractMaterial(true);
			break;
		}
	case ECropState::Sprout:
		{
			Mesh->SetRelativeLocation(
				UFarmingManager::GetDataByCropId(2).GetLocation());
			Mesh->SetRelativeScale3D(
				UFarmingManager::GetDataByCropId(2).GetScale());
			Mesh->SetStaticMesh(
				UFarmingManager::GetDataByCropId(2).GetStaticMesh());
			Mesh->SetMaterial(
				0, UFarmingManager::GetDataByCropId(2).GetMeshMaterial());
			break;
		}
	case ECropState::Stem:
		{
			Mesh->SetRelativeLocation(
				UFarmingManager::GetDataByCropId(3).GetLocation());
			Mesh->SetRelativeScale3D(
				UFarmingManager::GetDataByCropId(3).GetScale());
			Mesh->SetStaticMesh(
				UFarmingManager::GetDataByCropId(3).GetStaticMesh());
			Mesh->SetMaterial(
				0, UFarmingManager::GetDataByCropId(3).GetMeshMaterial());
			break;
		}
	case ECropState::Mature:
		{
			Mesh->SetRelativeScale3D(
				UFarmingManager::GetDataByCropId(MatureFarmingObjectId).
				GetScale());
			Mesh->SetStaticMesh(
				UFarmingManager::GetDataByCropId(MatureFarmingObjectId).
				GetStaticMesh());
			Mesh->SetRelativeLocation(
				UFarmingManager::GetDataByCropId(MatureFarmingObjectId).
				GetLocation());
			Mesh->SetMaterial(
				0, UFarmingManager::GetDataByCropId(MatureFarmingObjectId).
				GetMeshMaterial());
			break;
		}
	}
}

void ABaseCrop::SetOverlayInteractMaterial(bool value)
{
	if (value)
	{
		Mesh->SetOverlayMaterial(OverlayMaterial);
	}
	else
	{
		Mesh->SetOverlayMaterial(nullptr);
	}
}
