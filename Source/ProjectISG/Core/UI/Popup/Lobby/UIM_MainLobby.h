// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIModel.h"
#include "UIM_MainLobby.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTISG_API UUIM_MainLobby : public UBaseUIModel
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	class ULevelSequence* LobbyLS;

	UPROPERTY(EditAnywhere)
	class ALevelSequenceActor* LevelSequenceActor;
};
