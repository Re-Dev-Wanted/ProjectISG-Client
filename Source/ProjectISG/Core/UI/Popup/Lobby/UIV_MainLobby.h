// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "UIV_MainLobby.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTISG_API UUIV_MainLobby : public UBaseUIView
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnClickedOpenChooseCharacterUIButton();

	UFUNCTION()
	void OnClickedFindSessionUIButton();

	UFUNCTION()
	void OnClickedExitButton();

private:
#pragma region Widget
	UPROPERTY(meta = (BindWidget))
	class UButton* OpenChooseCharacterUIButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* OpenFindSessionUIButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ExitButton;
#pragma endregion
};
