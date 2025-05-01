// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "UIC_ProductInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTISG_API UUIC_ProductInfoWidget : public UBaseUIController
{
	GENERATED_BODY()

public:
	void SetProductInfo(const uint32 ProductId);

protected:
	virtual void InitializeController(UBaseUIView* NewView,
	                                  UBaseUIModel* NewModel) override;

private:
	UFUNCTION()
	void OnSelectProductData();
};
