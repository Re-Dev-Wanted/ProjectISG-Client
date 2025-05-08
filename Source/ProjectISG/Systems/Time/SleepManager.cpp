#include "SleepManager.h"

#include "Bed.h"
#include "TimeManager.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "ProjectISG/Contents/Diary/Component/DiaryComponent.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
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

	if (GetOwner()->HasAuthority() == false)
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
		//ForceSleep();
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

		// 침대에 각 플레이어를 배정 시킨다
		AssignBedEachPlayer();

		// 플레이어를 강제로 침대로 이동시킨다.
		SleepDelegate.Broadcast();

		// 플레이어 상태를 수면상태로 바꾼다
		ChangeAllPlayerSleepValue(true);

		// 시네마틱을 진행시킨다.
		if (bSleepCinematicStart == false)
		{
			LoggingToSleep();
		}

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
		if (bSleepCinematicStart)
		{
			LoggingToWakeUp();
		}

		ChangeAllPlayerWriteDiary(false);
		ChangeAllPlayerSleepValue(false);
		ChangeAllPlayerLieInBedValue(false);
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
		WakeUpDelegate.Broadcast();
		LoggingToWakeUp();
		ChangeAllPlayerWriteDiary(false);
	}
}

void USleepManager::AssignBedEachPlayer()
{
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

void USleepManager::ChangeAllPlayerSleepValue(bool value)
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
	UE_LOG(LogTemp, Warning, TEXT("sleep, localrole : %s"),
	       *FEnumUtil::GetClassEnumKeyAsString(TimeManager->GetLocalRole()));
	FDiaryLogParams LogParams;
	LogParams.ActionType = ELoggingActionType::DAY_CYCLE;
	LogParams.ActionName = ELoggingActionName::sleep;

	GetWorld()->GetGameInstance()->GetSubsystem<ULoggingSubSystem>()->
	            LoggingData(LogParams);
}

void USleepManager::LoggingToWakeUp()
{
	UE_LOG(LogTemp, Warning, TEXT("wakeup, localrole : %s"),
	       *FEnumUtil::GetClassEnumKeyAsString(TimeManager->GetLocalRole()));
	FDiaryLogParams LogParams;
	LogParams.ActionType = ELoggingActionType::DAY_CYCLE;
	LogParams.ActionName = ELoggingActionName::start_day;

	GetWorld()->GetGameInstance()->GetSubsystem<ULoggingSubSystem>()->
	            LoggingData(LogParams);

	GetWorld()->GetGameInstance()->GetSubsystem<ULoggingSubSystem>()->Flush();
}
