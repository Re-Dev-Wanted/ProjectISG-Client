// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "UIC_EndingChoice.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTISG_API UUIC_EndingChoice : public UBaseUIController
{
	GENERATED_BODY()

protected:
	virtual void AppearUI() override;

	UFUNCTION()
	void OnLeaveButtonClicked();
	
	UFUNCTION()
	void OnRemainButtonClicked();

	UFUNCTION()
	void MoveToLobby();

	UFUNCTION()
	void EndingLeaveSceneStart();

	UFUNCTION()
	void EndingRemainSceneStart();
private:
	
};
