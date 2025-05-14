// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "TeleportPoint.generated.h"

class UArrowComponent;

UCLASS()
class PROJECTISG_API ATeleportPoint : public AActor
{
	GENERATED_BODY()

public:
	ATeleportPoint();

	GETTER(UArrowComponent*, Point)

protected:
	UPROPERTY(VisibleAnywhere)
	UArrowComponent* Point;
};
