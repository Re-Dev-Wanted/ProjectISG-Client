#include "AT_EndCookingModeCinematic.h"

#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "MovieSceneSequencePlaybackSettings.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectISG/Contents/Cooking/Props/KitchenFurniture.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/Character/Player/Component/InteractionComponent.h"

UAT_EndCookingModeCinematic* UAT_EndCookingModeCinematic
::InitialEvent(
	UGameplayAbility* Ability, ULevelSequence* LevelSequence)
{
	UAT_EndCookingModeCinematic
		* NewTask = NewAbilityTask<
			UAT_EndCookingModeCinematic
		>(Ability);
	NewTask->LevelSequence = LevelSequence;

	return NewTask;
}

void UAT_EndCookingModeCinematic::Activate()
{
	Super::Activate();

	const AMainPlayerCharacter* MainPlayerCharacter = Cast<
		AMainPlayerCharacter>(GetAvatarActor());

	if (!MainPlayerCharacter)
	{
		EndTask();
		return;
	}

	AActor* HitResult = MainPlayerCharacter->GetInteractionComponent()->
	                                         GetTargetTraceResult().GetActor();

	TargetKitchen = Cast<AKitchenFurniture>(HitResult);
	if (!TargetKitchen)
	{
		EndTask();
		return;
	}

	AMainPlayerController* PC = MainPlayerCharacter->GetController<
		AMainPlayerController>();
	if (!PC)
	{
		return;
	}

	FMovieSceneSequencePlaybackSettings PlaybackSettings;
	PlaybackSettings.bAutoPlay = true;

	LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
		GetAvatarActor()->GetWorld(), LevelSequence,
		PlaybackSettings, LevelSequenceActor);

	LevelSequencePlayer->OnFinished.AddDynamic(this, &ThisClass::OnFinish);

	LevelSequenceActor->
		AddBindingByTag(FName(TEXT("Player")), GetAvatarActor());
	LevelSequenceActor->AddBindingByTag(FName(TEXT("Interactive")),
	                                    TargetKitchen);

	LevelSequenceActor->SetActorTransform(GetAvatarActor()->GetTransform());

	LevelSequencePlayer->Play();
}

void UAT_EndCookingModeCinematic
::ExternalConfirm(bool bEndTask)
{
	Super::ExternalConfirm(bEndTask);
}

void UAT_EndCookingModeCinematic::OnFinish()
{
	OnEndCookingModeCinematicEndNotified.Broadcast();
}
