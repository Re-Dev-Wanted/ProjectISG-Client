// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "UIV_ChooseCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTISG_API UUIV_ChooseCharacter : public UBaseUIView
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnClickedGameStart();

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* GameStart;
};
