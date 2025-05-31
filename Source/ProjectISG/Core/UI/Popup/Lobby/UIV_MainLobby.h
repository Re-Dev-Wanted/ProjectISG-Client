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
	void OnClickedGameStartButton();

	UFUNCTION()
	void OnClickedMultiplayButton();

	UFUNCTION()
	void OnClickedExitButton();

private:
#pragma region Widget
	UPROPERTY(meta = (BindWidget))
	class UButton* GameStart;

	UPROPERTY(meta = (BindWidget))
	class UButton* Multiplay;

	UPROPERTY(meta = (BindWidget))
	class UButton* Exit;
#pragma endregion
};
