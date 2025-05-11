// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "ProjectISG/Core/UI/HUD/Interactive/InteractiveItemUI.h"
#include "UIV_ExitInteractUI.generated.h"

UCLASS()
class PROJECTISG_API UUIV_ExitInteractUI : public UBaseUIView
{
	GENERATED_BODY()

public:
	GETTER(UInteractiveItemUI*, InteractiveUI)

protected:
	UPROPERTY(meta = (BindWidget))
	UInteractiveItemUI* InteractiveUI;
};
