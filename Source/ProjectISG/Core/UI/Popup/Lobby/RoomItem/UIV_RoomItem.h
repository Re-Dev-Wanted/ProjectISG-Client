// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "UIV_RoomItem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTISG_API UUIV_RoomItem : public UBaseUIView
{
	GENERATED_BODY()

public:
	void SetInfo(const FOnlineSessionSearchResult& SearchResult);

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnClickedJoinButton();
	
	void OnJoinSession(FName SessionName, EOnJoinSessionCompleteResult::Type Type);

private:
#pragma region Widget
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* RoomNameText;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentPlayerText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MaxPlayerText;
#pragma endregion

	FOnlineSessionSearchResult SessionSearchResult;
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;


};
