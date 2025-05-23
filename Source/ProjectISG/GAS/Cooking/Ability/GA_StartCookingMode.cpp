#include "GA_StartCookingMode.h"

#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Systems/Animation/Manager/LevelSequenceManager.h"
#include "Task/AT_StartCookingModeCinematic.h"

void UGA_StartCookingMode::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		GetAvatarActorFromActorInfo());

	Player->GetController<AMainPlayerController>()->PopUI();

	AT_StartCookingModeCinematic = UAT_StartCookingModeCinematic::InitialEvent(
		this, StartCookingCinematic);

	AT_StartCookingModeCinematic->OnStartCookingModeCinematicEndNotified.
	                              AddDynamic(this, &ThisClass::OnEndCinematic);
	AT_StartCookingModeCinematic->ReadyForActivation();
}

void UGA_StartCookingMode::OnEndCinematic()
{
	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		GetAvatarActorFromActorInfo());

	Player->GetController<AMainPlayerController>()->PushUI(
		EUIName::Popup_CookingRecipeUI);

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo,
	           false, false);
}
