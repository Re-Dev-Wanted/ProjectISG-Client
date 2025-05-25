// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainPlayerController.h"
#include "OnlineSessionSettings.h"
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
	void StartSceneEnd();
	
	UFUNCTION()
	void OpenMainLevel();
	
	void StartScene6(bool IsServerTravel);

	UFUNCTION(BlueprintCallable)
	void LoadingNextLevel(TSoftObjectPtr<UWorld> Level);

public:
	UPROPERTY(EditAnywhere, Category = Settings)
	FString StartSceneName;

	UPROPERTY(EditAnywhere, Category = Settings)
	FString StartQuestId;

	bool bIsServerTravel = false;

	FOnlineSessionSearchResult SessionSearchResult;
};
