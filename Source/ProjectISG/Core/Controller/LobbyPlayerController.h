// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "GameFramework/PlayerController.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "LobbyPlayerController.generated.h"

enum class EUIName : uint32;
/**
 * 
 */
UCLASS()
class PROJECTISG_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ALobbyPlayerController();

	void PushUI(const EUIName UIName);
	void PopUI();
	void ShowLoadingUI(bool bIsServerTravel);

	GETTER(TObjectPtr<class UUIManageComponent>, UIManageComponent)

	FOnlineSessionSearchResult SessionSearchResult;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UUIManageComponent> UIManageComponent;
};
