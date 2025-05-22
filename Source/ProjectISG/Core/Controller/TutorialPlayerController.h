// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainPlayerController.h"
#include "TutorialPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTISG_API ATutorialPlayerController : public AMainPlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void StartGranmaLevel();
};
