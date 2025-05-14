// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "ProjectISG/Utils/SessionUtil.h"
#include "UIC_MultiPlay.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTISG_API UUIC_MultiPlay : public UBaseUIController
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void CreateSession();

	UFUNCTION()
	void SearchGameAndJoinSession();

	void FindSession();

protected:
	UFUNCTION()
	void OnCompleteCreate(FName SessionName, bool IsCreateSuccess);

	UFUNCTION()
	void OnCompleteSearchAndTryJoin(bool IsSearchSuccess);

	UFUNCTION()
	void OnCompleteSearch(bool IsSearchSuccess);

	void OnJoinSession(FName SessionName,
	                   EOnJoinSessionCompleteResult::Type Type);

private:
	FSessionSearchData SessionSearchData;
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;

	UPROPERTY(EditAnywhere)
	uint8 MaxPlayer = 4;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUIV_RoomItem> RoomItemFactory;
};
