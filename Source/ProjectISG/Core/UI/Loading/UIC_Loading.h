// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "UIC_Loading.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTISG_API UUIC_Loading : public UBaseUIController
{
	GENERATED_BODY()

public:
	void CreateSession();
	
	void JoinSession();

protected:
	UFUNCTION()
	void OnCompleteCreate(FName SessionName, bool IsCreateSuccess);

	void OnJoinSession(FName SessionName, EOnJoinSessionCompleteResult::Type Type);
	
private:
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;
};
