// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCrop.h"
#include "TestCrop.generated.h"

UCLASS()
class PROJECTISG_API ATestCrop : public ABaseCrop
{
	GENERATED_BODY()

public:
	ATestCrop();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	
};
