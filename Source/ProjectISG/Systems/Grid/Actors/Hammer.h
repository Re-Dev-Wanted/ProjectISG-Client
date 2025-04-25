// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/GAS/Common/Object/BaseActor.h"
#include "Hammer.generated.h"

UCLASS()
class PROJECTISG_API AHammer : public ABaseActor
{
	GENERATED_BODY()

public:
	AHammer();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
