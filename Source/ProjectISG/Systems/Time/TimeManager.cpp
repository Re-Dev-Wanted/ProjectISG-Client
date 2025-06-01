#include "TimeManager.h"

#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "SleepManager.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/InteractionComponent.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/UI/Base/Components/UIManageComponent.h"
#include "ProjectISG/Core/UI/Gameplay/MainHUD/UI/UIC_MainHUD.h"
#include "ProjectISG/Core/UI/Gameplay/MainHUD/UI/UIV_MainHUD.h"
#include "ProjectISG/Core/UI/Gameplay/MainHUD/Widget/AlertModal/UIC_HUDAlertModalWidget.h"
#include "ProjectISG/Core/UI/Gameplay/MainHUD/Widget/AlertModal/UIV_HUDAlertModalWidget.h"
#include "ProjectISG/Core/UI/HUD/Time/UIC_Time.h"
#include "ProjectISG/Core/UI/HUD/Time/UIM_Time.h"
#include "ProjectISG/Core/UI/HUD/Time/UIV_Time.h"
#include "ProjectISG/Systems/Logging/LoggingEnum.h"
#include "ProjectISG/Systems/Logging/LoggingStruct.h"
#include "ProjectISG/Systems/Logging/LoggingSubSystem.h"
#include "ProjectISG/Utils/EnumUtil.h"


class ULoggingSubSystem;

ATimeManager::ATimeManager()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	Super::SetReplicateMovement(true);
	SetNetAddressable();
	bAlwaysRelevant = true;

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

	OnContentRestrictionTimeReached.AddDynamic(
		this, &ATimeManager::StopLevelSequence);
	OnContentRestrictionTimeReached.AddDynamic(
		this, &ATimeManager::ResetAllPlayerWidget);
	
	OnForceSleepTimeAlmostReached.AddDynamic(
		this, &ATimeManager::PushSleepAlertWidget);
	
	OnContentRestrictionCancelTimeReached.AddDynamic(
		this, &ATimeManager::SetIsInteractiveTrue);

	SleepManager->ForceSleepDelegate.AddDynamic(this, &ATimeManager::StopLevelSequence);
}

void ATimeManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsLobbyTimer)
	{
		UpdateCycleTime(DeltaTime);
		RotateSun();
		return;
	}

	if (!TimeStop && !SleepManager->GetbSleepCinematicStart())
	{
		if (HasAuthority())
		{
			UpdateCycleTime(DeltaTime);
			RotateSun();
			ChangeTOD();

			// 서버 시간 UI 업데이트
			if (CheckTimeUI())
			{
				if (TimeController->GetbInitialized() == false)
				{
					TimeController->UpdateDayText(Day, Month);
					TimeController->
						UpdateTimeImage(TimeModel->GetMorningIcon());
					TimeController->SetbInitialized(true);
				}
				else
				{
					TimeController->UpdateTimeText(Hour, Minute);
					TimeController->UpdateDayText(Day, Month);
				}
			}
		}
		else
		{
			// 클라이언트 시간 UI 업데이트
			if (CheckTimeUI())
			{
				if (TimeController->GetbInitialized() == false)
				{
					TimeController->UpdateDayText(Day, Month);
					TimeController->
						UpdateTimeImage(TimeModel->GetMorningIcon());
					TimeController->SetbInitialized(true);
				}
				else
				{
					TimeController->UpdateTimeText(Hour, Minute);
					TimeController->UpdateDayText(Day, Month);
				}
			}
		}
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

	if (Hour == 22 && Minute == 0 && Second == 0)
	{
		OnForceSleepTimeAlmostReached.Broadcast();
	}

	if (Hour == 20 && Minute == 0 && Second == 0)
	{
		OnContentRestrictionTimeReached.Broadcast();
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

	if (CheckTimeUI())
	{
		TimeController->UpdateDayText(Day, Month);
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

void ATimeManager::ChangeTOD()
{
	ETimeOfDay PreviousTimeOfDay = CurrentTimeOfDay; // 변경 전 상태 저장
	if (Hour >= 6 && Hour <= 12)
	{
		CurrentTimeOfDay = ETimeOfDay::Morning;
	}
	else if (Hour > 12 && Hour <= 18)
	{
		CurrentTimeOfDay = ETimeOfDay::Afternoon;
	}
	else if (Hour > 18 && Hour < 21)
	{
		CurrentTimeOfDay = ETimeOfDay::Evening;
	}
	else
	{
		CurrentTimeOfDay = ETimeOfDay::Night;
	}

	if (PreviousTimeOfDay != CurrentTimeOfDay)
	{
		UpdateTimeOfDay(CurrentTimeOfDay);
	}
}

void ATimeManager::UpdateTimeOfDay(ETimeOfDay TOD)
{
	if (CheckTimeUI())
	{
		switch (TOD)
		{
		case ETimeOfDay::Morning:
			{
				LoggingToMorning();
				TimeController->UpdateTimeImage(TimeModel->GetMorningIcon());
				break;
			}
		case ETimeOfDay::Afternoon:
			{
				LoggingToAfternoon();
				TimeController->UpdateTimeImage(TimeModel->GetAfternoonIcon());
				break;
			}
		case ETimeOfDay::Evening:
			{
				LoggingToEvening();
				TimeController->UpdateTimeImage(TimeModel->GetEveningIcon());
				break;
			}
		case ETimeOfDay::Night:
			{
				LoggingToNight();
				TimeController->UpdateTimeImage(TimeModel->GetNightIcon());
				break;
			}
		default:
			break;
		}
	}
	else
	{
		switch (TOD)
		{
		case ETimeOfDay::Morning:
			{
				LoggingToMorning();
				break;
			}
		case ETimeOfDay::Afternoon:
			{
				LoggingToAfternoon();
				break;
			}
		case ETimeOfDay::Evening:
			{
				LoggingToEvening();
				break;
			}
		case ETimeOfDay::Night:
			{
				LoggingToNight();
				break;
			}
		default:
			break;
		}
	}
}

void ATimeManager::OnRep_CurrentTimeOfDay()
{
	UpdateTimeOfDay(CurrentTimeOfDay);
}

bool ATimeManager::CheckTimeUI()
{
	AMainPlayerController* PC = Cast<AMainPlayerController>(
		GetWorld()->GetFirstPlayerController());

	if (PC && PC->GetUIManageComponent()->HasViewUI(EUIName::Gameplay_MainHUD))
	{
		if (TimeView == nullptr || TimeController == nullptr)
		{
			UUIC_MainHUD* MainHUDController = Cast<UUIC_MainHUD>(
				PC->GetUIManageComponent()->ControllerInstances[
					EUIName::Gameplay_MainHUD]);
			UUIV_MainHUD* MainHUDView = Cast<
				UUIV_MainHUD>(MainHUDController->GetView());

			TimeView = MainHUDView->GetTimeUI();
			TimeController = Cast<UUIC_Time>(TimeView->GetController());
			TimeModel = Cast<UUIM_Time>(TimeController->GetModel());
		}

		return true;
	}

	return false;
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

void ATimeManager::ResetAllPlayerWidget()
{
	if (HasAuthority() == false)
	{
		return;
	}

	AGameStateBase* GameState = GetWorld()->GetGameState();
	for (APlayerState* PlayerState : GameState->PlayerArray)
	{
		AMainPlayerController* PC = Cast<AMainPlayerController>(
			PlayerState->GetPlayerController());
		if (PC)
		{
			PC->Client_ResetWidgetAndPushContentsTimeAlert();
			PlayerState->GetPawn<AMainPlayerCharacter>()->GetInteractionComponent()->SetIsRestrictionTime(true);
		}
	}
}

void ATimeManager::StopLevelSequence()
{
	if (HasAuthority() == false)
	{
		return;
	}

	ALevelSequenceActor* CurrentLevelSequenceActor = Cast<ALevelSequenceActor>(
		UGameplayStatics::GetActorOfClass(GetWorld(),
		                                  ALevelSequenceActor::StaticClass()));
	if (CurrentLevelSequenceActor)
	{
		CurrentLevelSequenceActor->GetSequencePlayer()->Stop();
		CurrentLevelSequenceActor->Destroy();
	}
}

void ATimeManager::PushSleepAlertWidget()
{
	if (HasAuthority() == false)
	{
		return;
	}

	AGameStateBase* GameState = GetWorld()->GetGameState();
	for (APlayerState* PlayerState : GameState->PlayerArray)
	{
		AMainPlayerController* PC = Cast<AMainPlayerController>(
			PlayerState->GetPlayerController());
		if (PC)
		{
			PC->Client_PushForceSleepTimeAlert();
		}
	}
}

void ATimeManager::SetIsInteractiveTrue()
{
	if (HasAuthority() == false)
	{
		return;
	}

	AGameStateBase* GameState = GetWorld()->GetGameState();
	for (APlayerState* PlayerState : GameState->PlayerArray)
	{
		PlayerState->GetPawn<AMainPlayerCharacter>()->GetInteractionComponent()
				   ->SetIsRestrictionTime(false);
		PlayerState->GetPawn<AMainPlayerCharacter>()->GetInteractionComponent()
		           ->SetIsInteractive(true);
	}
}

void ATimeManager::LoggingToMorning()
{
	FDiaryLogParams LogParams;
	LogParams.ActionType = ELoggingActionType::TIME_EVENT;
	LogParams.ActionName = ELoggingActionName::morning;

	GetWorld()->GetGameInstance()->GetSubsystem<ULoggingSubSystem>()->
	            LoggingData(LogParams);
}

void ATimeManager::LoggingToAfternoon()
{
	FDiaryLogParams LogParams;
	LogParams.ActionType = ELoggingActionType::TIME_EVENT;
	LogParams.ActionName = ELoggingActionName::afternoon;

	GetWorld()->GetGameInstance()->GetSubsystem<ULoggingSubSystem>()->
	            LoggingData(LogParams);
}

void ATimeManager::LoggingToEvening()
{
	FDiaryLogParams LogParams;
	LogParams.ActionType = ELoggingActionType::TIME_EVENT;
	LogParams.ActionName = ELoggingActionName::evening;

	GetWorld()->GetGameInstance()->GetSubsystem<ULoggingSubSystem>()->
	            LoggingData(LogParams);
}

void ATimeManager::LoggingToNight()
{
	FDiaryLogParams LogParams;
	LogParams.ActionType = ELoggingActionType::TIME_EVENT;
	LogParams.ActionName = ELoggingActionName::night;

	GetWorld()->GetGameInstance()->GetSubsystem<ULoggingSubSystem>()->
	            LoggingData(LogParams);
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
	DateText += FTimespan(Day - 1, Hour, Minute, Second);

	return DateText.ToString();
}

FString ATimeManager::GetDateDisplayText() const
{
	FDateTime DateText(1, 1, 1, 0, 0, 0);
	DateText += FTimespan(Day - 1, Hour, Minute, Second);

	return DateText.ToFormattedString(TEXT("%Y년 %m월 %d일"));
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
