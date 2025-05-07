#include "AT_FailFishingCinematic.h"

#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "MovieSceneSequencePlaybackSettings.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"


UAT_FailFishingCinematic* UAT_FailFishingCinematic::InitialEvent(UGameplayAbility* Ability,
	ULevelSequence* LevelSequence)
{
	UAT_FailFishingCinematic* NewTask = NewAbilityTask<UAT_FailFishingCinematic>(Ability);
	NewTask->LevelSequence = LevelSequence;

	return NewTask;
}

void UAT_FailFishingCinematic::Activate()
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

	LevelSequencePlayer->OnFinished.AddDynamic(this, &UAT_FailFishingCinematic::OnFinish);
	LevelSequenceActor->AddBindingByTag(FName(TEXT("Player")), GetAvatarActor());

	LevelSequenceActor->SetActorTransform(GetAvatarActor()->GetTransform());

	LevelSequencePlayer->Play();
}

void UAT_FailFishingCinematic::ExternalConfirm(bool bEndTask)
{
	Super::ExternalConfirm(bEndTask);
}

void UAT_FailFishingCinematic::OnFinish()
{
	OnFailFishingCinematicEndNotified.Broadcast();
}
