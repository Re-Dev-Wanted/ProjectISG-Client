#include "AT_LogWithScreenShot.h"

#include "Kismet/GameplayStatics.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Systems/Logging/LoggingSubSystem.h"
#include "ProjectISG/Systems/Logging/Component/ScreenShotComponent.h"
#include "ProjectISG/Systems/Time/TimeManager.h"

UAT_LogWithScreenShot* UAT_LogWithScreenShot::InitialEvent(
	UGameplayAbility* Ability)
{
	UAT_LogWithScreenShot* NewTask = NewAbilityTask<
		UAT_LogWithScreenShot>(Ability);

	return NewTask;
}

void UAT_LogWithScreenShot::Activate()
{
	Super::Activate();

	const ATimeManager* TimeManager = Cast<ATimeManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(),
		                                  ATimeManager::StaticClass()));
	LogParams.CurrentDate = TimeManager->GetDateText();

	if (!IsLogWithScreenShot)
	{
		GetWorld()->GetGameInstance()->GetSubsystem<ULoggingSubSystem>()->
		            LoggingData(LogParams);

		EndTask();

		return;
	}

	FOnCaptureFrameNotified CaptureFrameNotified;
	CaptureFrameNotified.BindLambda(
		[this](const TArray64<uint8>& FileBinary)
		{
			LogParams.File = FileBinary;

			GetWorld()->GetGameInstance()->GetSubsystem<ULoggingSubSystem>()->
			            LoggingData(LogParams);

			EndTask();
		});

	Cast<AMainPlayerCharacter>(GetAvatarActor())->
		GetScreenShotComponent()->SaveCaptureFrameImage(CaptureFrameNotified);
}
