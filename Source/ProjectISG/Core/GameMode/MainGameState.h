// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "MainGameState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTISG_API AMainGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	GETTER(FString, SessionId);

private:
	FString SessionId = TEXT("e1827901-2536-4fb9-b76a-ca8e149015cb");
};
