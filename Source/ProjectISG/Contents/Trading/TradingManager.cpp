// Fill out your copyright notice in the Description page of Project Settings.


#include "TradingManager.h"

bool UTradingManager::IsInitialize;
TArray<FProductStruct> UTradingManager::ProductData;

void UTradingManager::Initialize()
{
	if (IsInitialize)
	{
		return;
	}

	IsInitialize = true;

	const static ConstructorHelpers::FObjectFinder<UDataTable> ProductDataTable(
		TEXT("'/Game/Contents/Trading/Data/DT_ProductData.DT_ProductData'"));

	if (ProductDataTable.Succeeded())
	{
		TArray<FProductStruct*> TempProductDataList;
		ProductDataTable.Object->GetAllRows<FProductStruct>(
			TEXT(""), TempProductDataList);

		for (const FProductStruct* InfoProduct : TempProductDataList)
		{
			// 데이터 테이블에서 product data 넣기
			ProductData.Add(*InfoProduct);
		}
	}
}

uint32 UTradingManager::GetProductPriceById(uint32 Id)
{
	for (int i = 0, Count = ProductData.Num(); i < Count; i++)
	{
		if (ProductData[i].GetProductId() == Id)
		{
			return ProductData[i].GetProductPrice();
		}
	}

	return 0;
}

FProductStruct UTradingManager::GetProductDataById(uint32 Id)
{
	for (int i = 0, Count = ProductData.Num(); i < Count; i++)
	{
		if (ProductData[i].GetProductId() == Id)
		{
			return ProductData[i];
		}
	}

	return FProductStruct();
}
