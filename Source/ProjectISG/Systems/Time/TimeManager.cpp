// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeManager.h"

#include "Components/DirectionalLightComponent.h"
#include "Net/UnrealNetwork.h"
#include "ProjectISG/Utils/EnumUtil.h"


ATimeManager::ATimeManager()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetNetAddressable();

	Sun = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("Sun"));
	if (Sun)
	{
		SetRootComponent(Sun);
		Sun->SetWorldRotation(FRotator(-90,0,0));
	}
}

void ATimeManager::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATimeManager, Seconds);
	DOREPLIFETIME(ATimeManager, Minutes);
	DOREPLIFETIME(ATimeManager, Hours);
	DOREPLIFETIME(ATimeManager, Days);
	DOREPLIFETIME(ATimeManager, Months);
	DOREPLIFETIME(ATimeManager, Year);
}

void ATimeManager::BeginPlay()
{
	Super::BeginPlay();

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

	if (!TimeStop)
	{
		UpdateCycleTime(DeltaTime);
		RotateSun();
	}

	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Tab))
	{
		TimeStop = !TimeStop;
	}
}

void ATimeManager::UpdateCycleTime(float DeltaTime)
{
	Seconds += SpeedOfTime * DeltaTime;

	if (Seconds >= 60)
	{
		Seconds = 0;
		Minutes++;
	}

	if (Minutes >= 60)
	{
		Minutes = 0;
		Hours++;
	}

	if (Hours >= 24)
	{
		Hours = 0;
		UpdateCycleDate();
	}

	UE_LOG(LogTemp, Warning, TEXT("localRole : %s , Days : %d, %d:%d:%f"), *FEnumUtil::GetClassEnumKeyAsString(GetLocalRole()), Days, Hours, Minutes, Seconds);
}

void ATimeManager::UpdateCycleDate()
{
	Days++;

	if (Days > DaysInMonths[Months - 1])
	{
		Days = 1;
		Months++;
	}

	if (Months > 12)
	{
		Months = 1;
		Year++;
	}
}

void ATimeManager::RotateSun()
{
	if (HasAuthority() == false) return;
	float TotalMinutes = Hours * 60.0f + Minutes;

	// 06(해 뜨는 시간) ~ 24(해 지는 시간)을 기준으로 offset을 통해 일출과 일몰 시간을 정해줌
	float SunPitch = FMath::GetMappedRangeValueClamped(
		FVector2d(0.f, 1440.0f),
		FVector2d((90.f + (6 - SunriseTime) * 15.f), 360.f + ((24 - SunsetTime) * 15.f)), 
		TotalMinutes);

	// 0 ~ 360 범위로 정규화
	SunPitch = FMath::Fmod(SunPitch, 360.0f);
	FRotator newRot = FRotator(SunPitch, 0.f, 0.f);
	NetMulticast_RotateSun(newRot);
}

void ATimeManager::NetMulticast_RotateSun_Implementation(FRotator newRot)
{
	SetActorRotation(newRot);
}

