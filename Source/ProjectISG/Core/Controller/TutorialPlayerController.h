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
	UFUNCTION()
	void OpenMainLevel();
	
	void StartScene6(bool IsServerTravel);

public:
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UWorld> MoveLevel;

	UPROPERTY(EditAnywhere)
	FString StartSceneName;

	bool bIsServerTravel = false;

	FOnlineSessionSearchResult SessionSearchResult;
};
