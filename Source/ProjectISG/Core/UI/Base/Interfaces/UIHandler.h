// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UIHandler.generated.h"

UINTERFACE(BlueprintType)
class UUIHandler : public UInterface
{
	GENERATED_BODY()
};

class PROJECTISG_API IUIHandler
{
	GENERATED_BODY()

public:
	virtual void OnClosed() = 0;
};
