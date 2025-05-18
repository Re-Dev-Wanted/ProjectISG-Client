// Fill out your copyright notice in the Description page of Project Settings.

#include "AT_StartCraftingMode.h"

#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "MovieSceneSequencePlaybackSettings.h"
#include "ProjectISG/Contents/Building/Props/Workbench.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/InteractionComponent.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"

UAT_StartCraftingMode* UAT_StartCraftingMode::InitialEvent(UGameplayAbility* Ability, ULevelSequence* LevelSequence)
{
	UAT_StartCraftingMode* NewTask = NewAbilityTask<UAT_StartCraftingMode>(Ability);
	NewTask->LevelSequence = LevelSequence;

	return NewTask;
}

void UAT_StartCraftingMode::Activate()
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

	TargetWorkbench = Cast<AWorkbench>(HitResult);

	if (!TargetWorkbench)
	{
		EndTask();
		return;
	}

	AMainPlayerController* PC = MainPlayerCharacter->GetController<AMainPlayerController>();
	
	if (!PC)
	{
		EndTask();
		return;
	}

	FMovieSceneSequencePlaybackSettings PlaybackSettings;
	PlaybackSettings.bAutoPlay = true;

	LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
		GetAvatarActor()->GetWorld(), LevelSequence,
		PlaybackSettings, LevelSequenceActor);

	LevelSequencePlayer->OnFinished.AddDynamic(this, &ThisClass::OnFinish);

	PC->SetIgnoreMoveInput(true);
	PC->SetIgnoreLookInput(true);
	
	LevelSequenceActor->AddBindingByTag(FName(TEXT("Player")), GetAvatarActor());
	LevelSequenceActor->AddBindingByTag(FName(TEXT("Target")), TargetWorkbench);

	LevelSequenceActor->SetActorTransform(GetAvatarActor()->GetTransform());

	LevelSequencePlayer->Play();
}

void UAT_StartCraftingMode::OnFinish()
{
	OnStartCraftingCinematicEndNotified.Broadcast();
}
