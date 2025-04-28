// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "UIV_ProductInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTISG_API UUIV_ProductInfoWidget : public UBaseUIView
{
	GENERATED_BODY()

public:
	GETTER(class UButton*, ProductSelectButton);
	GETTER(class UImage*, ProductThumbnail);
	GETTER(class UTextBlock*, ProductName);

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* ProductSelectButton;

	UPROPERTY(meta = (BindWidget))
	class UImage* ProductThumbnail;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ProductName;
};
