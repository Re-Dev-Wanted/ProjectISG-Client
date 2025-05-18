#include "AT_EndCraftingMode.h"

#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "MovieSceneSequencePlaybackSettings.h"
#include "ProjectISG/Contents/Building/Props/Workbench.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/InteractionComponent.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"

UAT_EndCraftingMode* UAT_EndCraftingMode::InitialEvent(UGameplayAbility* Ability, ULevelSequence* LevelSequence, AWorkbench* Workbench)
{
	UAT_EndCraftingMode* NewTask = NewAbilityTask<UAT_EndCraftingMode>(Ability);
	NewTask->LevelSequence = LevelSequence;
	NewTask->TargetWorkbench = Workbench;

	return NewTask;
}

void UAT_EndCraftingMode::Activate()
{
	Super::Activate();

	const AMainPlayerCharacter* MainPlayerCharacter = Cast<
		AMainPlayerCharacter>(GetAvatarActor());

	if (!MainPlayerCharacter)
	{
		EndTask();
		return;
	}

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
	
	LevelSequenceActor->AddBindingByTag(FName(TEXT("Player")), GetAvatarActor());
	LevelSequenceActor->AddBindingByTag(FName(TEXT("Target")), TargetWorkbench);

	LevelSequenceActor->SetActorTransform(GetAvatarActor()->GetTransform());

	LevelSequencePlayer->Play();
}

void UAT_EndCraftingMode::OnFinish()
{
	OnEndCraftingCinematicEndNotified.Broadcast();
}
