﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIModel.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "UIM_EndingChoice.generated.h"

class UEndingMoviePlayer;
/**
 * 
 */
UCLASS()
class PROJECTISG_API UUIM_EndingChoice : public UBaseUIModel
{
	GENERATED_BODY()

public:
	GETTER(FString, LeaveScene)
	GETTER(FString, RemainScene)
	GETTER(TSoftObjectPtr<UWorld>, LobbyLevel)
	GETTER(FString, EndingVideoScene)
	GETTER(TSubclassOf<UEndingMoviePlayer>, MoviePlayer)
	GETTER(USoundWave*, EndingBGM)
private:
	UPROPERTY(EditAnywhere)
	FString LeaveScene;

	UPROPERTY(EditAnywhere)
	FString RemainScene;

	UPROPERTY(EditAnywhere)
	FString EndingVideoScene;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UWorld> LobbyLevel;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UEndingMoviePlayer> MoviePlayer;

	UPROPERTY(EditAnywhere)
	USoundWave* EndingBGM;
};
