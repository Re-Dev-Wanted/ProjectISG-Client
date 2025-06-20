﻿#include "SleepManager.h"

#include "Bed.h"
#include "TimeManager.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "ProjectISG/Contents/Diary/Component/DiaryComponent.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Systems/Logging/LoggingEnum.h"
#include "ProjectISG/Systems/Logging/LoggingStruct.h"
#include "ProjectISG/Systems/Logging/LoggingSubSystem.h"
#include "ProjectISG/Utils/EnumUtil.h"

USleepManager::USleepManager()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);
	SetNetAddressable();
}

void USleepManager::InitializeComponent()
{
	Super::InitializeComponent();
	TimeManager = Cast<ATimeManager>(GetOwner());
}

void USleepManager::BeginPlay()
{
	Super::BeginPlay();
}

void USleepManager::GetLifetimeReplicatedProps(
	TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, bSleepCinematicStart);
}

void USleepManager::TickComponent(float DeltaTime, ELevelTick TickType,
                                  FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (GetOwner()->HasAuthority() == false || bUseSleepManager == false)
	{
		return;
	}

	if (bSleepCinematicStart)
	{
		SleepCinematic(DeltaTime);
	}
	else
	{
		Sleep();
		if (bUseForceSleep)
		{
			ForceSleep();
		}
	}
}

void USleepManager::Sleep()
{
	if (CheckAllPlayerIsLieInBed() && TimeManager->GetHour() >= CanSleepTime)
	{
		ChangeAllPlayerSleepValue(true);
		if (bSleepCinematicStart == false)
		{
			OpenDiaryDelegate.Broadcast();
			LoggingToSleep();
		}

		bSleepCinematicStart = true;
		TimeManager->StopTime(true);
	}
}

void USleepManager::ForceSleep()
{
	if (TimeManager->GetHour() >= 23)
	{
		// 시간을 멈춘다
		TimeManager->StopTime(true);

		LoadHouseLevel();

		// 서버의 수면 로그 보낸다.
		if (bSleepCinematicStart == false)
		{
			LoggingToSleep();
		}

		// 시네마틱을 진행시킨다.
		bSleepCinematicStart = true;
	}
}

void USleepManager::SleepCinematic(float DeltaTime)
{
	if (CheckAllPlayerWriteDiary() == true)
	{
		CinematicElapsedTime += DeltaTime;
	}

	if (CinematicElapsedTime >= CinematicEndTime)
	{
		TimeManager->StopTime(false);
		TimeManager->AddSleepTimeToCrop.Broadcast();
		WakeUpDelegate.Broadcast();
		TimeManager->OnContentRestrictionCancelTimeReached.Broadcast();

		if (bSleepCinematicStart)
		{
			LoggingToWakeUp();
		}

		AGameStateBase* GameState = GetWorld()->GetGameState();
		for (APlayerState* PlayerState : GameState->PlayerArray)
		{
			AMainPlayerCharacter* player = Cast<AMainPlayerCharacter>(
				PlayerState->GetPawn());
			if (player)
			{
				player->SetbIsSleep(false);
				player->SetbLieInBed(false);
				player->GetDiaryComponent()->SetbWriteDiary(false);
			}
		}
		TimeManager->ChangeDayBySleep();

		bSleepCinematicStart = false;
		CinematicElapsedTime = 0.f;
	}
}

void USleepManager::OnRep_SleepCinematicStart()
{
	if (bSleepCinematicStart)
	{
		LoggingToSleep();
	}
	else
	{
		LoggingToWakeUp();
		ChangeAllPlayerWriteDiary(false);
	}
}

void USleepManager::LoadHouseLevel()
{
	UE_LOG(LogTemp, Warning, TEXT("LoadHouseLevel()"));

	FLatentActionInfo LatentActionInfo;
	UGameplayStatics::LoadStreamLevelBySoftObjectPtr(GetWorld(),
		HouseLevel, true, false, LatentActionInfo);

	FTimerHandle TimerHandle;
	TWeakObjectPtr<ThisClass> WeakThis = this;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [WeakThis]()
	{
		if (WeakThis.IsValid())
		{
			WeakThis.Get()->AssignBedEachPlayer();
		}
	}, 0.25, false);
}

void USleepManager::AssignBedEachPlayer()
{
	TArray<AActor*> Beds;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABed::StaticClass(),
	                                      Beds);

	if (Beds.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("맵에 배치된 침대가 없습니다."));
		return;
	}

	int32 idx = 0;
	AGameStateBase* GameState = GetWorld()->GetGameState();
	for (APlayerState* PlayerState : GameState->PlayerArray)
	{
		ABed* bed = Cast<ABed>(Beds[idx]);
		AMainPlayerCharacter* player = Cast<AMainPlayerCharacter>(
			PlayerState->GetPawn());
		AMainPlayerController* PC = Cast<AMainPlayerController>(player->GetController());

		if (player && bed)
		{
			PC->PushUI(EUIName::Modal_TempLoadingUI);
			bed->SetMainPlayer(player);
			player->SetbIsSleep(true);
			player->SetbLieInBed(true);
			idx++;
		}
	}

	// 침대에 눕는 어빌리티를 실행시킨다.
	ForceSleepDelegate.Broadcast();
	OpenDiaryDelegate.Broadcast();

}

void USleepManager::ChangeAllPlayerSleepValue(bool value)
{
	AGameStateBase* GameState = GetWorld()->GetGameState();
	for (APlayerState* PlayerState : GameState->PlayerArray)
	{
		AMainPlayerCharacter* player = Cast<AMainPlayerCharacter>(
			PlayerState->GetPawn());
		AMainPlayerController* PC = Cast<AMainPlayerController>(player->GetController());
		if (player)
		{
			player->SetbIsSleep(value);
			PC->PushUI(EUIName::Modal_TempLoadingUI);
		}
	}
}

void USleepManager::ChangeAllPlayerLieInBedValue(bool value)
{
	AGameStateBase* GameState = GetWorld()->GetGameState();
	for (APlayerState* PlayerState : GameState->PlayerArray)
	{
		AMainPlayerCharacter* player = Cast<AMainPlayerCharacter>(
			PlayerState->GetPawn());
		if (player)
		{
			player->SetbLieInBed(value);
		}
	}
}

bool USleepManager::CheckAllPlayerIsLieInBed()
{
	AGameStateBase* GameState = GetWorld()->GetGameState();
	for (APlayerState* PlayerState : GameState->PlayerArray)
	{
		AMainPlayerCharacter* player = Cast<AMainPlayerCharacter>(
			PlayerState->GetPawn());
		if (player)
		{
			if (player->GetbLieInBed() == false)
			{
				return false;
			}
		}
	}
	return true;
}

bool USleepManager::CheckAllPlayerWriteDiary()
{
	AGameStateBase* GameState = GetWorld()->GetGameState();
	for (APlayerState* PlayerState : GameState->PlayerArray)
	{
		AMainPlayerCharacter* player = Cast<AMainPlayerCharacter>(
			PlayerState->GetPawn());
		AMainPlayerController* PC = Cast<AMainPlayerController>(player->GetController());
		if (player)
		{
			if (player->GetDiaryComponent()->GetbWriteDiary() == false)
			{
				return false;
			}
		}
	}
	return true;
}

void USleepManager::ChangeAllPlayerWriteDiary(bool value)
{
	AGameStateBase* GameState = GetWorld()->GetGameState();
	for (APlayerState* PlayerState : GameState->PlayerArray)
	{
		AMainPlayerCharacter* player = Cast<AMainPlayerCharacter>(
			PlayerState->GetPawn());
		if (player)
		{
			player->GetDiaryComponent()->SetbWriteDiary(value);
		}
	}
}

void USleepManager::LoggingToSleep()
{
	FDiaryLogParams LogParams;
	LogParams.ActionType = ELoggingActionType::DAY_CYCLE;
	LogParams.ActionName = ELoggingActionName::sleep;

	GetWorld()->GetGameInstance()->GetSubsystem<ULoggingSubSystem>()->
	            LoggingData(LogParams);

	GetWorld()->GetGameInstance()->GetSubsystem<ULoggingSubSystem>()->Flush();
}

void USleepManager::LoggingToWakeUp()
{
	FDiaryLogParams LogParams;
	LogParams.ActionType = ELoggingActionType::DAY_CYCLE;
	LogParams.ActionName = ELoggingActionName::start_day;

	GetWorld()->GetGameInstance()->GetSubsystem<ULoggingSubSystem>()->
	            LoggingData(LogParams);
}
