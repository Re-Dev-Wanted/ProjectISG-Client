// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "UIC_MainLobby.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTISG_API UUIC_MainLobby : public UBaseUIController
{
	GENERATED_BODY()

public:
	void PlayLobbyLS();

	UFUNCTION()
	void OnFinishLS();
};
