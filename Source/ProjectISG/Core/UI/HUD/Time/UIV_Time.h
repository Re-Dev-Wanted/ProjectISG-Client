// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "UIV_Time.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTISG_API UUIV_Time : public UBaseUIView
{
	GENERATED_BODY()

public:
	GETTER_SETTER(class UImage*, TimeImage)
	GETTER_SETTER(class UTextBlock*, TimeText)
	GETTER_SETTER(class UTextBlock*, DayText)

private:
	UPROPERTY(meta = (BindWidget))
	class UImage* TimeImage;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TimeText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DayText;
};
