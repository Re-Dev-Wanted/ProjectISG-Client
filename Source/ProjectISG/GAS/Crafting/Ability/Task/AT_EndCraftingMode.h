#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "ProjectISG/Contents/Building/Props/Workbench.h"
#include "AT_EndCraftingMode.generated.h"

class AWorkbench;
class ULevelSequencePlayer;
class ALevelSequenceActor;
class ULevelSequence;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndCraftingCinematicEndNotified);

UCLASS()
class PROJECTISG_API UAT_EndCraftingMode : public UAbilityTask
{
	GENERATED_BODY()

public:
	static UAT_EndCraftingMode* InitialEvent(UGameplayAbility* Ability, ULevelSequence* LevelSequence, AWorkbench* Workbench);

	FOnEndCraftingCinematicEndNotified OnEndCraftingCinematicEndNotified;

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
