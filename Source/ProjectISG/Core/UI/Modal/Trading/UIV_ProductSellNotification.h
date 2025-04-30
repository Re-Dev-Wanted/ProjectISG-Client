// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "UIV_ProductSellNotification.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTISG_API UUIV_ProductSellNotification : public UBaseUIView
{
	GENERATED_BODY()

public:
	GETTER(class UButton*, SellButton);

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* SellButton;
};
