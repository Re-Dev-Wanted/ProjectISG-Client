// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AT_SuccessFishingCinematic.generated.h"

class ALevelSequenceActor;
class ULevelSequencePlayer;
class ULevelSequence;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSuccessFishingCinematicEndNotified);

UCLASS()
class PROJECTISG_API UAT_SuccessFishingCinematic : public UAbilityTask
{
	GENERATED_BODY()

public:
	static UAT_SuccessFishingCinematic* InitialEvent(UGameplayAbility* Ability, ULevelSequence* LevelSequence);

	FOnSuccessFishingCinematicEndNotified OnSuccessFishingCinematicEndNotified;

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
