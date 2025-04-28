// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProductStruct.h"
#include "UObject/Object.h"
#include "TradingManager.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTISG_API UTradingManager : public UObject
{
	GENERATED_BODY()

public:
	static void Initialize();
	static TArray<FProductStruct> GetProductData() { return ProductData; }

private:
	static TArray<FProductStruct> ProductData;
	static bool IsInitialize;
};
