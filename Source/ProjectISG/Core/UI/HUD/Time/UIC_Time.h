// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "UIC_Time.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTISG_API UUIC_Time : public UBaseUIController
{
	GENERATED_BODY()

public:
	void UpdateTimeText(int32 ChangeHour, int32 ChangeMinute);
	void UpdateDayText(int32 ChangeDay, int32 ChangeMonth);
	void UpdateTimeImage(class UTexture2D* ChangeImage);

	GETTER_SETTER(bool, bInitialized)

private:
	bool bInitialized = false;
};
