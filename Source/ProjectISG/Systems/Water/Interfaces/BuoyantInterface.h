// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BuoyantInterface.generated.h"

UINTERFACE(MinimalAPI)
class UBuoyantInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECTISG_API IBuoyantInterface
{
	GENERATED_BODY()

public:
	// 현재 액터가 부력을 받을 수 있는 상태인지
	virtual bool CanApplyBuoyancy() const = 0; 

	// 부력 세기 조정
	virtual float GetBuoyancyScale() const = 0;

	virtual float GetActorBottomZ() const = 0;
};
