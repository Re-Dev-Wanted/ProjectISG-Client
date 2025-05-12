#include "AT_PlayCinematic.h"

#include "LevelSequenceActor.h"
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

	FMovieSceneSequencePlaybackSettings PlaybackSettings;
	PlaybackSettings.bAutoPlay = true;

	LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
		GetAvatarActor()->GetWorld(), LevelSequence, PlaybackSettings
		, LevelSequenceActor);

	LevelSequenceActor->SetReplicates(false);
	LevelSequencePlayer->OnFinished.
	                     AddDynamic(this, &ThisClass::OnEndCinematic);
	OnPlayCinematicOnReadyNotified.ExecuteIfBound(LevelSequenceActor);

	LevelSequencePlayer->Play();
}

void UAT_PlayCinematic::OnEndCinematic()
{
	if (!OnPlayCinematicEndNotified.ExecuteIfBound())
	{
		UE_LOG(LogTemp, Display, TEXT("시네마틱 종료에 대한 설정 값 없음"));
	}

	LevelSequenceActor->Destroy();
}
