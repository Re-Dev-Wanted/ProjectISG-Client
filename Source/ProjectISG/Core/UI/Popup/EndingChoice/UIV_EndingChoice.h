// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "UIV_EndingChoice.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTISG_API UUIV_EndingChoice : public UBaseUIView
{
	GENERATED_BODY()

public:
	GETTER(UButton*, LeaveThisWorldButton)
	GETTER(UButton*, RemainThisWorldButton)

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* LeaveThisWorldButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* RemainThisWorldButton;

};
