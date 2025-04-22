#include "GA_StartCookingMode.h"

// #include "LevelSequencePlayer.h"
// #include "LevelSequenceActor.h"
// #include "MovieSceneSequencePlaybackSettings.h"

void UGA_StartCookingMode::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UE_LOG(LogTemp, Display, TEXT("시작시작하다"))

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);

	// FMovieSceneSequencePlaybackSettings PlaybackSettings;
	// PlaybackSettings.bAutoPlay = true;
	//
	// ALevelSequenceActor* LevelSequenceActor;
	// // 새로운 레벨 시퀀스 플레이어를 만든다.
	// const TObjectPtr<ULevelSequencePlayer> LevelSequencePlayer
	// 	= ULevelSequencePlayer::CreateLevelSequencePlayer(
	// 		GetAvatarActorFromActorInfo()->GetWorld(), StartCookingCinematic,
	// 		PlaybackSettings, LevelSequenceActor);
	//
	// // LevelSequencePlayer->OnFinished.AddDynamic(this, &ThisClass::OnFinish);
	//
	// // LevelSequenceActor->
	// // 	AddBindingByTag(FName(TEXT("Player")), GetAvatarActor());
	// LevelSequenceActor->AddBindingByTag(FName(TEXT("Interactive")),
	//                                     GetAvatarActorFromActorInfo());
	//
	// LevelSequenceActor->SetActorTransform(
	// 	GetAvatarActorFromActorInfo()->GetTransform());
	//
	// // 레벨 시퀀스 플레이어를 실행한다.
	// LevelSequencePlayer->Play();
}
