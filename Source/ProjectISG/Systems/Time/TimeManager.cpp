// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeManager.h"

#include "Bed.h"
#include "Components/DirectionalLightComponent.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Utils/EnumUtil.h"


ATimeManager::ATimeManager()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	Super::SetReplicateMovement(true);
	SetNetAddressable();

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Sun = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("Sun"));
	if (Sun)
	{
		Sun->SetupAttachment(Root);
		Sun->SetIntensity(3.0f);
		Sun->SetLightColor(FLinearColor(255, 255, 255, 255));
	}
	
	Moon = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("Moon"));
	if (Moon)
	{
		Moon->SetupAttachment(Root);
		Moon->SetIntensity(1.0f);
		Moon->SetLightColor(FLinearColor(0, 0, 0, 255));
	}
}

void ATimeManager::GetLifetimeReplicatedProps(
	TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATimeManager, Second);
	DOREPLIFETIME(ATimeManager, Minute);
	DOREPLIFETIME(ATimeManager, Hour);
	DOREPLIFETIME(ATimeManager, Day);
	DOREPLIFETIME(ATimeManager, Month);
	DOREPLIFETIME(ATimeManager, Year);
	DOREPLIFETIME(ATimeManager, TimeStop);
}

void ATimeManager::BeginPlay()
{
	Super::BeginPlay();

	// 1~12월의 일 수 등록
	for (int32 i = 0; i < 12; i++)
	{
		if (i == 0 || i == 2 || i == 4 || i == 6 || i == 7 || i == 9 || i == 11)
		{
			DaysInMonths.Add(31);
		}
		else if (i == 3 || i == 5 || i == 8 || i == 10)
		{
			DaysInMonths.Add(30);
		}
		else if (i == 1)
		{
			DaysInMonths.Add(28);
		}
	}

	Player = Cast<AMainPlayerCharacter>(
		GetWorld()->GetFirstPlayerController()->GetPawn());
}

void ATimeManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 잘 때 해당 코드를 실행하라


	if (HasAuthority() == false)
	{
		return;
	}

	if (!TimeStop && !bSleepCinematicStart)
	{
		UpdateCycleTime(DeltaTime);
		RotateSun();
		Sleep();
		//ForceSleep();
	}
	else if (bSleepCinematicStart)
	{
		// 시네마틱 시간 확인 함수
		SleepCinematic(DeltaTime);
	}
}

void ATimeManager::UpdateCycleTime(float DeltaTime)
{
	Second += SpeedOfTime * DeltaTime;

	if (Second >= 60)
	{
		Second = 0;
		Minute++;
	}

	if (Minute >= 60)
	{
		Minute = 0;
		Hour++;
	}

	if (Hour >= 24)
	{
		Hour = 0;
		UpdateCycleDate();
	}

	// UE_LOG(LogTemp, Warning, TEXT("localRole : %s , Day : %d, %d:%d:%f"),
	//        *FEnumUtil::GetClassEnumKeyAsString(GetLocalRole()), Day, Hour,
	//        Minute, Second);
}

void ATimeManager::UpdateCycleDate()
{
	Day++;

	if (Day > DaysInMonths[Month - 1])
	{
		Day = 1;
		Month++;
	}

	if (Month > 12)
	{
		Month = 1;
		Year++;
	}
}

void ATimeManager::RotateSun()
{
	float TotalMinutes = Hour * 60.0f + Minute;

	// 06(해 뜨는 시간) ~ 24(해 지는 시간)을 기준으로 offset을 통해 일출과 일몰 시간을 정해줌
	float SunPitch = FMath::GetMappedRangeValueClamped(
		FVector2d(0.f, 1440.0f),
		FVector2d((90.f + (6 - SunriseTime) * 15.f),
		          360.f + ((24 - SunsetTime) * 15.f)),
		TotalMinutes);

	// 0 ~ 360 범위로 정규화
	SunPitch = FMath::Fmod(SunPitch, 360.0f);
	FRotator newRot = FRotator(SunPitch, 0.f, 0.f);
	SunRotation = newRot;
}

void ATimeManager::StopTime(bool value)
{
	if (value)
	{
		TimeStop = true;
	}
	else
	{
		TimeStop = false;
	}
}

void ATimeManager::Sleep()
{
	if (CheckAllPlayerIsLieOnBed() && Hour >= 9)
	{
		ChangeAllPlayerSleepValue(true);
		bSleepCinematicStart = true;
	}
}

void ATimeManager::ForceSleep()
{
	// 강제 수면시간 저녁 11시로 하드 코딩
	if (Hour >= 23)
	{
		// 침대에 각 플레이어를 배정 시킨다
		AssignBedEachPlayer();

		// 플레이어를 강제로 침대로 이동시킨다.
		SleepDelegate.Broadcast();

		// 플레이어 상태를 수면상태로 바꾼다
		ChangeAllPlayerSleepValue(true);

		// 시네마틱을 진행시킨다.
		bSleepCinematicStart = true;
	}
}

void ATimeManager::SleepCinematic(float DeltaTime)
{
	CinematicElapsedTime += DeltaTime;
	if (CinematicElapsedTime >= CinematicEndTime)
	{
		ChangeAllPlayerSleepValue(false);
		bSleepCinematicStart = false;
		Hour = 6;
		Minute = 0;
		Second = 0;
		Day++;
		CinematicElapsedTime = 0.f;
	}
}

void ATimeManager::AssignBedEachPlayer()
{
	// 강제 수면 시, 각 플레이어에게 침대를 배정해주는 작업
	if (HasAuthority() == false)
	{
		return;
	}

	TArray<AActor*> Beds;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABed::StaticClass(),
	                                      Beds);

	int32 idx = 0;
	AGameStateBase* GameState = GetWorld()->GetGameState();
	for (APlayerState* PlayerState : GameState->PlayerArray)
	{
		ABed* bed = Cast<ABed>(Beds[idx]);
		AMainPlayerCharacter* player = Cast<AMainPlayerCharacter>(
			PlayerState->GetPawn());
		if (player && bed)
		{
			bed->SetMainPlayer(player);
			idx++;
		}
	}
}

void ATimeManager::ChangeAllPlayerSleepValue(bool value)
{
	AGameStateBase* GameState = GetWorld()->GetGameState();
	for (APlayerState* PlayerState : GameState->PlayerArray)
	{
		AMainPlayerCharacter* player = Cast<AMainPlayerCharacter>(
			PlayerState->GetPawn());
		if (player)
		{
			player->SetbIsSleep(value);
		}
	}
}

bool ATimeManager::CheckAllPlayerIsLieOnBed()
{
	AGameStateBase* GameState = GetWorld()->GetGameState();
	for (APlayerState* PlayerState : GameState->PlayerArray)
	{
		AMainPlayerCharacter* player = Cast<AMainPlayerCharacter>(
			PlayerState->GetPawn());
		if (player)
		{
			if (player->GetbLieOnBed() == false)
				return false;
		}
	}

	return true;
}

void ATimeManager::ChangeTimeToSleepTime()
{
	Hour = 23;
}

void ATimeManager::ChangeTimeToCanSleepTime()
{
	Hour = 9;
}
