// Fill out your copyright notice in the Description page of Project Settings.

#include "AT_SuccessFishingCinematic.h"

#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "MovieSceneSequencePlaybackSettings.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/GAS/Fishing/GA_ReelInLine.h"

UAT_SuccessFishingCinematic* UAT_SuccessFishingCinematic::InitialEvent(UGameplayAbility* Ability,
                                                                       ULevelSequence* LevelSequence)
{
	UAT_SuccessFishingCinematic* NewTask = NewAbilityTask<UAT_SuccessFishingCinematic>(Ability);
	NewTask->LevelSequence = LevelSequence;

	return NewTask;
}

void UAT_SuccessFishingCinematic::Activate()
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

	LevelSequencePlayer->OnFinished.AddUniqueDynamic(this, 
	&UAT_SuccessFishingCinematic::OnFinish);
	LevelSequenceActor->AddBindingByTag(FName(TEXT("Player")), GetAvatarActor());

	LevelSequenceActor->SetActorTransform(GetAvatarActor()->GetTransform());

	LevelSequencePlayer->Play();
}

void UAT_SuccessFishingCinematic::ExternalConfirm(bool bEndTask)
{
	Super::ExternalConfirm(bEndTask);
}

void UAT_SuccessFishingCinematic::OnFinish()
{
	if (OnSuccessFishingCinematicEndNotified.IsBound())
	{
		OnSuccessFishingCinematicEndNotified.Broadcast();
	}
}
