#include "SleepManager.h"

#include "Bed.h"
#include "TimeManager.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Systems/Logging/LoggingEnum.h"
#include "ProjectISG/Systems/Logging/LoggingStruct.h"


USleepManager::USleepManager()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);
}

void USleepManager::InitializeComponent()
{
	Super::InitializeComponent();
	TimeManager = Cast<ATimeManager>(GetOwner());
}


// Called when the game starts
void USleepManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void USleepManager::TickComponent(float DeltaTime, ELevelTick TickType,
                                  FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TimeManager->HasAuthority() == false)
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
		ForceSleep();
	}
}

void USleepManager::Sleep()
{
	if (CheckAllPlayerIsLieOnBed() && TimeManager->GetHour() >= CanSleepTime)
	{
		ChangeAllPlayerSleepValue(true);
		bSleepCinematicStart = true;
		LoggingToSleep();
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
		bSleepCinematicStart = true;

		LoggingToSleep();
	}
}

void USleepManager::SleepCinematic(float DeltaTime)
{
	CinematicElapsedTime += DeltaTime;
	if (CinematicElapsedTime >= CinematicEndTime)
	{
		TimeManager->StopTime(false);
		TimeManager->AddSleepTimeToCrop.Broadcast();
		WakeUpDelegate.Broadcast();
		LoggingToWakeUp();
		
		ChangeAllPlayerSleepValue(false);
		bSleepCinematicStart = false;
		TimeManager->ChangeDayBySleep();
		CinematicElapsedTime = 0.f;
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

bool USleepManager::CheckAllPlayerIsLieOnBed()
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

void USleepManager::LoggingToSleep()
{
	FDiaryLogParams LogParams;
	LogParams.ActionType = ELoggingActionType::DAY_CYCLE;
	LogParams.ActionName = ELoggingActionName::sleep;
}

void USleepManager::LoggingToWakeUp()
{
	FDiaryLogParams LogParams;
	LogParams.ActionType = ELoggingActionType::DAY_CYCLE;
	LogParams.ActionName = ELoggingActionName::start_day;
}
