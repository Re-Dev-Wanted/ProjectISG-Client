#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AT_PlayCinematic.generated.h"

class ALevelSequenceActor;
class ULevelSequencePlayer;
class ULevelSequence;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayCinematicPreStartNotified
											, UAT_PlayCinematic*, AbilityTask);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayCinematicOnReadyNotified
											, ALevelSequenceActor*
											, LevelSequenceActor);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayCinematicEndNotified);

UCLASS()
class PROJECTISG_API UAT_PlayCinematic : public UAbilityTask
{
	GENERATED_BODY()

public:
	static UAT_PlayCinematic* InitialEvent(UGameplayAbility* Ability
											, ULevelSequence* LevelSequence
											, ALevelSequenceActor*
											LevelSequenceActor);

	FOnPlayCinematicPreStartNotified OnPlayCinematicPreStartNotified;
	FOnPlayCinematicOnReadyNotified OnPlayCinematicOnReadyNotified;
	FOnPlayCinematicEndNotified OnPlayCinematicEndNotified;

protected:
	virtual void Activate() override;

private:
	UPROPERTY()
	TObjectPtr<ULevelSequencePlayer> LevelSequencePlayer;

	UPROPERTY()
	TObjectPtr<ULevelSequence> LevelSequence;

	UPROPERTY()
	ALevelSequenceActor* LevelSequenceActor;

	UFUNCTION()
	void OnEndCinematic();
};
