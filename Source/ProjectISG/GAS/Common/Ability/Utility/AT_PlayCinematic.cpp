#include "AT_PlayCinematic.h"
#include "LevelSequencePlayer.h"
#include "MovieSceneSequencePlaybackSettings.h"

UAT_PlayCinematic* UAT_PlayCinematic::InitialEvent(UGameplayAbility* Ability
													, ULevelSequence*
													LevelSequence
													, ALevelSequenceActor*
													LevelSequenceActor)
{
	UAT_PlayCinematic* NewTask = NewAbilityTask<UAT_PlayCinematic>(Ability);
	NewTask->LevelSequence = LevelSequence;
	NewTask->LevelSequenceActor = LevelSequenceActor;

	return NewTask;
}


void UAT_PlayCinematic::Activate()
{
	Super::Activate();

	OnPlayCinematicPreStartNotified.Broadcast(this);

	FMovieSceneSequencePlaybackSettings PlaybackSettings;
	PlaybackSettings.bAutoPlay = true;

	LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
		GetAvatarActor()->GetWorld(), LevelSequence, PlaybackSettings
		, LevelSequenceActor);

	LevelSequencePlayer->OnFinished.
						AddDynamic(this, &ThisClass::OnEndCinematic);
	OnPlayCinematicOnReadyNotified.Broadcast(LevelSequenceActor);

	LevelSequencePlayer->Play();
}

void UAT_PlayCinematic::OnEndCinematic()
{
	OnPlayCinematicEndNotified.Broadcast();
}
