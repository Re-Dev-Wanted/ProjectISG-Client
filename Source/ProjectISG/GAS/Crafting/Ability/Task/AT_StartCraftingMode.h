#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AT_StartCraftingMode.generated.h"

class AWorkbench;
class ULevelSequencePlayer;
class ALevelSequenceActor;
class ULevelSequence;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartCraftingCinematicEndNotified);

UCLASS()
class PROJECTISG_API UAT_StartCraftingMode : public UAbilityTask
{
	GENERATED_BODY()

public:
	static UAT_StartCraftingMode* InitialEvent(UGameplayAbility* Ability, ULevelSequence* LevelSequence);

	FOnStartCraftingCinematicEndNotified OnStartCraftingCinematicEndNotified;

	virtual void Activate() override;

protected:
	UPROPERTY()
	TObjectPtr<AWorkbench> TargetWorkbench;
	
	UPROPERTY()
	TObjectPtr<ULevelSequencePlayer> LevelSequencePlayer;
	
	UPROPERTY()
	TObjectPtr<ULevelSequence> LevelSequence;

	UPROPERTY()
	ALevelSequenceActor* LevelSequenceActor;

	UFUNCTION()
	void OnFinish();
};
