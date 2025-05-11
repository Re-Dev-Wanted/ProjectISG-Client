#include "GA_EndCookingMode.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectISG/Core/Character/Player/Component/InteractionComponent.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Systems/Logging/LoggingEnum.h"
#include "ProjectISG/Systems/Logging/LoggingStruct.h"
#include "ProjectISG/Systems/Logging/LoggingSubSystem.h"
#include "Task/AT_EndCookingModeCinematic.h"

void UGA_EndCookingMode::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		GetAvatarActorFromActorInfo());

	Player->GetController<AMainPlayerController>()->PopUI();

	AT_EndCookingModeCinematic = UAT_EndCookingModeCinematic::InitialEvent(
		this, EndCookingCinematic);
	AT_EndCookingModeCinematic->OnEndCookingModeCinematicEndNotified.
	                            AddDynamic(this, &ThisClass::OnEndCinematic);
	AT_EndCookingModeCinematic->ReadyForActivation();
}

void UGA_EndCookingMode::OnEndCinematic()
{
	AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		GetAvatarActorFromActorInfo());

	AMainPlayerController* PC = Player->GetController<
		AMainPlayerController>();

	PC->PushUI(EUIName::Gameplay_MainHUD);
	PC->SetIgnoreLookInput(false);
	PC->SetViewTargetWithBlend(Player, 0.5f);

	UnlockPlayer();

	LoggingToEndCook();

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo,
	           false, false);
}

void UGA_EndCookingMode::UnlockPlayer()
{
	AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		GetAvatarActorFromActorInfo());

	Player->bUseControllerRotationYaw = true;
	Player->GetInteractionComponent()->SetIsInteractive(true);
	Player->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	Player->GetCameraComponent()->Activate();
}

void UGA_EndCookingMode::LoggingToEndCook()
{
	FDiaryLogParams LogParams;
	LogParams.Location = "요리장";
	LogParams.ActionType = ELoggingActionType::COOKING;
	LogParams.ActionName = ELoggingActionName::finish_cooking;

	GetWorld()->GetGameInstance()->GetSubsystem<ULoggingSubSystem>()->
	            LoggingData(LogParams);

	GetWorld()->GetGameInstance()->GetSubsystem<ULoggingSubSystem>()->Flush();
}
