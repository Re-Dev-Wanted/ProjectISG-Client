// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "UIV_MultiPlay.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTISG_API UUIV_MultiPlay : public UBaseUIView
{
	GENERATED_BODY()

public:
	GETTER(class UScrollBox*, RoomList)

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnClickedCreateRoomButton();

	UFUNCTION()
	void OnClickedJoinSessionButton();

private:
#pragma region Widget
	UPROPERTY(meta = (BindWidget))
	class UButton* CreateRoom;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinSession;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* RoomList;
#pragma endregion
};
