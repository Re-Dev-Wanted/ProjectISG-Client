#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AT_PlayCinematic.generated.h"

class UAT_PlayCinematic;
class ALevelSequenceActor;
class ULevelSequencePlayer;
class ULevelSequence;

DECLARE_DELEGATE_OneParam(FOnPlayCinematicOnReadyNotified,
                          ALevelSequenceActor*);

DECLARE_DELEGATE(FOnPlayCinematicEndNotified);

UCLASS()
class PROJECTISG_API UAT_PlayCinematic : public UAbilityTask
{
	GENERATED_BODY()

public:
	static UAT_PlayCinematic* InitialEvent(UGameplayAbility* Ability
	                                       , ULevelSequence* LevelSequence
	                                       , ALevelSequenceActor*
	                                       LevelSequenceActor);

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
