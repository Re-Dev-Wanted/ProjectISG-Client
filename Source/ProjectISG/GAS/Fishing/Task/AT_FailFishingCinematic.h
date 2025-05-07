// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AT_FailFishingCinematic.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFailFishingCinematicEndNotified);

class ALevelSequenceActor;
class ULevelSequencePlayer;
class ULevelSequence;

UCLASS()
class PROJECTISG_API UAT_FailFishingCinematic : public UAbilityTask
{
	GENERATED_BODY()

public:
	static UAT_FailFishingCinematic* InitialEvent(UGameplayAbility* Ability, ULevelSequence* LevelSequence);

	FOnFailFishingCinematicEndNotified OnFailFishingCinematicEndNotified;

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
