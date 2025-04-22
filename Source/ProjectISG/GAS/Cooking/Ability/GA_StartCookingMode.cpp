#include "GA_StartCookingMode.h"

#include "Task/AT_StartCookingModeCinematic.h"

void UGA_StartCookingMode::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AT_StartCookingModeCinematic = UAT_StartCookingModeCinematic::InitialEvent(
		this, StartCookingCinematic);
	AT_StartCookingModeCinematic->OnStartCookingModeCinematicEndNotified.
	                              AddDynamic(this, &ThisClass::OnEndCinematic);
	AT_StartCookingModeCinematic->ReadyForActivation();
}

void UGA_StartCookingMode::OnEndCinematic()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo,
	           false, false);
}
