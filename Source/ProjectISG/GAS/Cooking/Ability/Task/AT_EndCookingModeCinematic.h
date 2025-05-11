#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"

#include "AT_EndCookingModeCinematic.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndCookingModeCinematicEndNotified);

class AKitchenFurniture;
class ULevelSequence;
class ALevelSequenceActor;
class ULevelSequencePlayer;

UCLASS()
class PROJECTISG_API UAT_EndCookingModeCinematic : public UAbilityTask
{
	GENERATED_BODY()

public:
	static UAT_EndCookingModeCinematic* InitialEvent(
		UGameplayAbility* Ability, ULevelSequence* LevelSequence);

	FOnEndCookingModeCinematicEndNotified
	OnEndCookingModeCinematicEndNotified;

	virtual void Activate() override;

	virtual void ExternalConfirm(bool bEndTask) override;

private:
	UPROPERTY()
	TObjectPtr<AKitchenFurniture> TargetKitchen;

	UPROPERTY()
	TObjectPtr<ULevelSequencePlayer> LevelSequencePlayer;

	UPROPERTY()
	ALevelSequenceActor* LevelSequenceActor;

	UPROPERTY()
	TObjectPtr<ULevelSequence> LevelSequence;

	UFUNCTION()
	void OnFinish();
};
