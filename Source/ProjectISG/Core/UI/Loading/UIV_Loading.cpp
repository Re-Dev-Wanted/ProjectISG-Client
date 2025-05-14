// Fill out your copyright notice in the Description page of Project Settings.


#include "UIV_Loading.h"

#include "UIC_Loading.h"
#include "ProjectISG/Core/Controller/LobbyPlayerController.h"

void UUIV_Loading::NativeConstruct()
{
	Super::NativeConstruct();

	IsCreated = true;
}

void UUIV_Loading::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (IsCreated)
	{
		UUIC_Loading* LoadingController = Cast<UUIC_Loading>(GetController());
		
		if (LoadingController->GetIsServerTravel())
		{
			LoadingController->CreateSession();
		}
		else
		{
			LoadingController->JoinSession();
		}

		IsCreated = false;
	}
}
