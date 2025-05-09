#include "AT_PlayCinematic.h"

#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "MovieSceneSequencePlaybackSettings.h"
#include "ProjectISG/Core/Character/BaseCharacter.h"

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

	if (ABaseCharacter* PlayerBase = Cast<ABaseCharacter>(GetAvatarActor()))
	{
		PlayerBase->PlayCinematic(LevelSequencePlayer);
		if (PlayerBase->HasAuthority())
		{
			PlayerBase->Client_PlayCinematic(LevelSequencePlayer);
		}
	}
}

void UAT_PlayCinematic::OnEndCinematic()
{
	OnPlayCinematicEndNotified.Broadcast();

	// TODO: Pooling으로 대체할 필요 있음
	LevelSequenceActor->Destroy();
}
