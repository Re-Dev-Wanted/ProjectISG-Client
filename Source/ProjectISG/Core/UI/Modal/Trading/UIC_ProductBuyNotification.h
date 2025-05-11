// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "UIC_ProductBuyNotification.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTISG_API UUIC_ProductBuyNotification : public UBaseUIController
{
	GENERATED_BODY()

protected:
	virtual void InitializeController(UBaseUIView* NewView,
	                                  UBaseUIModel* NewModel) override;

	UFUNCTION()
	void OnClickedBuyButton();

private:
	int32 FindItemPrice(class UUIM_ProductBuyNotification* ProductBuyNotificationModel);

	void LoggingToBuyItem();
};
