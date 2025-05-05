// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeManager.h"

#include "SleepManager.h"
#include "Net/UnrealNetwork.h"
#include "ProjectISG/Systems/Logging/LoggingEnum.h"
#include "ProjectISG/Systems/Logging/LoggingStruct.h"


ATimeManager::ATimeManager()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	Super::SetReplicateMovement(true);
	SetNetAddressable();

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	SleepManager = CreateDefaultSubobject<USleepManager>(TEXT("SleepManager"));

	SetRootComponent(Root);
}

void ATimeManager::GetLifetimeReplicatedProps(
	TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATimeManager, CurrentTimeOfDay);
	DOREPLIFETIME(ATimeManager, Second);
	DOREPLIFETIME(ATimeManager, Minute);
	DOREPLIFETIME(ATimeManager, Hour);
	DOREPLIFETIME(ATimeManager, Day);
	DOREPLIFETIME(ATimeManager, Month);
	DOREPLIFETIME(ATimeManager, Year);
	DOREPLIFETIME(ATimeManager, TimeStop);
	DOREPLIFETIME(ATimeManager, SunRotation);
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
}

void ATimeManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority() == false)
	{
		return;
	}

	if (!TimeStop && !SleepManager->GetbSleepCinematicStart())
	{
		UpdateCycleTime(DeltaTime);
		CheckTimeOfDay();
		RotateSun();
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
	const float TotalMinutes = Hour * 60.0f + Minute;

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
		TimeStoppedTime = Hour;
		UE_LOG(LogTemp, Warning, TEXT("시간 정지"));
	}
	else
	{
		TimeStop = false;
		UE_LOG(LogTemp, Warning, TEXT("시간 정지 해제"));
	}
}

void ATimeManager::CheckTimeOfDay()
{
	if (Hour >= 6 && Hour <= 12)
	{
		if (CurrentTimeOfDay != ETimeOfDay::Morning)
		{
			ChangeCurrentTimeOfDay(ETimeOfDay::Morning);
		}
	}
	else if (Hour > 12 && Hour <= 18)
	{
		if (CurrentTimeOfDay != ETimeOfDay::Afternoon)
		{
			ChangeCurrentTimeOfDay(ETimeOfDay::Afternoon);
		}
	}
	else if (Hour > 18 && Hour < 24)
	{
		if (CurrentTimeOfDay != ETimeOfDay::Evening)
		{
			ChangeCurrentTimeOfDay(ETimeOfDay::Evening);
		}
	}
	else
	{
		if (CurrentTimeOfDay != ETimeOfDay::Night)
		{
			ChangeCurrentTimeOfDay(ETimeOfDay::Night);
		}
	}
}

void ATimeManager::ChangeCurrentTimeOfDay(ETimeOfDay ChangeTimeOfDay)
{
	switch (ChangeTimeOfDay)
	{
	case ETimeOfDay::Morning:
		{
			CurrentTimeOfDay = ChangeTimeOfDay;
			LoggingToMorning();
		}
	case ETimeOfDay::Afternoon:
		{
			CurrentTimeOfDay = ChangeTimeOfDay;
			LoggingToAfternoon();
		}
	case ETimeOfDay::Evening:
		{
			CurrentTimeOfDay = ChangeTimeOfDay;
			LoggingToEvening();
		}
	case ETimeOfDay::Night:
		{
			CurrentTimeOfDay = ChangeTimeOfDay;
			LoggingToNight();
		}
	}
}

void ATimeManager::LoggingToMorning()
{
	FDiaryLogParams LogParams;
	LogParams.ActionType = ELoggingActionType::TIME_EVENT;
	LogParams.ActionName = ELoggingActionName::morning;
}

void ATimeManager::LoggingToAfternoon()
{
	FDiaryLogParams LogParams;
	LogParams.ActionType = ELoggingActionType::TIME_EVENT;
	LogParams.ActionName = ELoggingActionName::afternoon;
}

void ATimeManager::LoggingToEvening()
{
	FDiaryLogParams LogParams;
	LogParams.ActionType = ELoggingActionType::TIME_EVENT;
	LogParams.ActionName = ELoggingActionName::evening;
}

void ATimeManager::LoggingToNight()
{
	FDiaryLogParams LogParams;
	LogParams.ActionType = ELoggingActionType::TIME_EVENT;
	LogParams.ActionName = ELoggingActionName::night;
}

void ATimeManager::ChangeDayBySleep()
{
	Hour = 6;
	Minute = 0;
	Second = 0;
	Day++;
}

FString ATimeManager::GetDateText() const
{
	FDateTime DateText(1, 1, 1, 0, 0, 0);
	DateText += FTimespan(Day - 1, 0, 0, 0);

	return DateText.ToString(TEXT("%Y.%m.%d"));
}

uint32 ATimeManager::GetTotalPlayingDay() const
{
	const uint32 YearToDay = (Year - 1) * 365;

	uint32 MonthOfDay = 0;
	for (int i = 1; i <= Month - 1; i++)
	{
		MonthOfDay += DaysInMonths[i - 1];
	}

	return YearToDay + MonthOfDay + Day;
}
