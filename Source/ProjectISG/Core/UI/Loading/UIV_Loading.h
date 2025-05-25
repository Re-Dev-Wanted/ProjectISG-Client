// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "UIV_Loading.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTISG_API UUIV_Loading : public UBaseUIView
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	GETTER_SETTER(bool, CreateSession)
	
private:
	bool CreateSession = false;
};
