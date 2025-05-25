// Fill out your copyright notice in the Description page of Project Settings.


#include "UIV_Loading.h"

#include "ProjectISG/Core/ISGGameInstance.h"

void UUIV_Loading::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUIV_Loading::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	// 로비에서 참가나 방 만들기 실행 후 작동 안함
	if (CreateSession)
	{
		UISGGameInstance* GameInstance = Cast<UISGGameInstance>(GetWorld()->GetGameInstance());
		
		if (GameInstance->GetIsServerTravel())
		{
			GameInstance->CreateSession();
		}
		else
		{
			GameInstance->JoinFoundSession();
		}
		CreateSession = false;
	}
}
