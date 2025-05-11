#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"

#include "AT_StartCookingModeCinematic.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartCookingModeCinematicEndNotified);

class AKitchenFurniture;
class ULevelSequence;
class ALevelSequenceActor;
class ULevelSequencePlayer;

UCLASS()
class PROJECTISG_API UAT_StartCookingModeCinematic : public UAbilityTask
{
	GENERATED_BODY()

public:
	static UAT_StartCookingModeCinematic* InitialEvent(
		UGameplayAbility* Ability, ULevelSequence* LevelSequence);

	FOnStartCookingModeCinematicEndNotified
	OnStartCookingModeCinematicEndNotified;

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
