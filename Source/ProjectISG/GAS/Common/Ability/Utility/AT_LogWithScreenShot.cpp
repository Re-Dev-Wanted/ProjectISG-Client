#include "AT_LogWithScreenShot.h"

#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Systems/Logging/LoggingSubSystem.h"
#include "ProjectISG/Systems/Logging/Component/ScreenShotComponent.h"

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
			if (!FileBinary.IsEmpty())
			{
				LogParams.File = FileBinary;
			}

			GetWorld()->GetGameInstance()->GetSubsystem<ULoggingSubSystem>()->
			            LoggingData(LogParams);

			EndTask();
		});

	Cast<AMainPlayerCharacter>(GetAvatarActor())->
		GetScreenShotComponent()->SaveCaptureFrameImage(
			this, CaptureFrameNotified);
}
