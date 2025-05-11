#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AT_StartFishingCinematic.generated.h"

class ALevelSequenceActor;
class ULevelSequencePlayer;
class ULevelSequence;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartFishingCinematicEndNotified);

UCLASS()
class PROJECTISG_API UAT_StartFishingCinematic : public UAbilityTask
{
	GENERATED_BODY()

public:
	static UAT_StartFishingCinematic* InitialEvent(UGameplayAbility* Ability, ULevelSequence* LevelSequence);

	FOnStartFishingCinematicEndNotified OnStartFishingCinematicEndNotified;

	virtual void Activate() override;

	virtual void ExternalConfirm(bool bEndTask) override;

protected:
	UPROPERTY()
	TObjectPtr<ULevelSequencePlayer> LevelSequencePlayer;
	
	UPROPERTY()
	TObjectPtr<ULevelSequence> LevelSequence;

	UPROPERTY()
	ALevelSequenceActor* LevelSequenceActor;

	UFUNCTION()
	void OnFinish();
	
};
