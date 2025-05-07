#include "AT_StartFishingCinematic.h"

#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "MovieSceneSequencePlaybackSettings.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"

UAT_StartFishingCinematic* UAT_StartFishingCinematic::InitialEvent(UGameplayAbility* Ability,
                                                                   ULevelSequence* LevelSequence)
{
	UAT_StartFishingCinematic* NewTask = NewAbilityTask<UAT_StartFishingCinematic>(Ability);
	NewTask->LevelSequence = LevelSequence;

	return NewTask;
}

void UAT_StartFishingCinematic::Activate()
{
	Super::Activate();

	const AMainPlayerCharacter* MainPlayerCharacter = Cast<
		AMainPlayerCharacter>(GetAvatarActor());

	if (!MainPlayerCharacter)
	{
		EndTask();
		return;
	}

	FMovieSceneSequencePlaybackSettings PlaybackSettings;
	PlaybackSettings.bAutoPlay = true;

	LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
		GetAvatarActor()->GetWorld(), LevelSequence,
		PlaybackSettings, LevelSequenceActor);

	LevelSequencePlayer->OnFinished.AddDynamic(this, &UAT_StartFishingCinematic::OnFinish);
	LevelSequenceActor->AddBindingByTag(FName(TEXT("Player")), GetAvatarActor());

	LevelSequenceActor->SetActorTransform(GetAvatarActor()->GetTransform());

	LevelSequencePlayer->Play();
}

void UAT_StartFishingCinematic::ExternalConfirm(bool bEndTask)
{
	Super::ExternalConfirm(bEndTask);
}

void UAT_StartFishingCinematic::OnFinish()
{
	OnStartFishingCinematicEndNotified.Broadcast();
}
