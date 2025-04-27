#include "GA_CookingQTEAction.h"

#include "LevelSequenceActor.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/GAS/Common/Ability/Utility/AT_PlayCinematic.h"

void UGA_CookingQTEAction::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle
	, const FGameplayAbilityActorInfo* ActorInfo
	, const FGameplayAbilityActivationInfo ActivationInfo
	, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		GetAvatarActorFromActorInfo());
	Player->GetController<AMainPlayerController>()->PushUI(
		EUIName::Popup_CookingQTE);

	RemainQTEQueue.Empty();
	for (int i = 0; i < QTESequenceList.Num(); i++)
	{
		RemainQTEQueue.Enqueue(QTESequenceList[i]);
	}

	PlayNextSequence();
}

void UGA_CookingQTEAction::PlayNextSequence()
{
	AT_PlayCinematic = UAT_PlayCinematic::InitialEvent(
		this, *RemainQTEQueue.Peek(), LevelSequenceActor);

	AT_PlayCinematic->OnPlayCinematicOnReadyNotified.Clear();
	AT_PlayCinematic->OnPlayCinematicOnReadyNotified.AddDynamic(
		this, &ThisClass::OnPlayReadySequence);
	AT_PlayCinematic->OnPlayCinematicEndNotified.Clear();
	AT_PlayCinematic->OnPlayCinematicEndNotified.AddDynamic(
		this, &ThisClass::OnEndSequence);

	AT_PlayCinematic->ReadyForActivation();
}

void UGA_CookingQTEAction::OnPlayReadySequence(
	ALevelSequenceActor* TargetSequenceActor)
{
	TargetSequenceActor->AddBindingByTag(FName(TEXT("Player"))
										, GetAvatarActorFromActorInfo());
}

void UGA_CookingQTEAction::EndAbility(const FGameplayAbilitySpecHandle Handle
									, const FGameplayAbilityActorInfo* ActorInfo
									, const FGameplayAbilityActivationInfo
									ActivationInfo, bool bReplicateEndAbility
									, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility
					, bWasCancelled);

	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		GetAvatarActorFromActorInfo());
	Player->GetController<AMainPlayerController>()->PopUI();
}

void UGA_CookingQTEAction::OnEndSequence()
{
	RemainQTEQueue.Pop();
	if (RemainQTEQueue.IsEmpty())
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo
					, true, false);
		return;
	}

	PlayNextSequence();
}
