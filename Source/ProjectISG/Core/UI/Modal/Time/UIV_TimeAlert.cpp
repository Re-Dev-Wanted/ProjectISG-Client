// Fill out your copyright notice in the Description page of Project Settings.


#include "UIV_TimeAlert.h"

#include "UIC_TimeAlert.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"

void UUIV_TimeAlert::NativeConstruct()
{
	Super::NativeConstruct();

	UUIC_TimeAlert* TimeAlertController = Cast<UUIC_TimeAlert>(GetController());
	AMainPlayerController* PC = Cast<AMainPlayerController>(TimeAlertController->GetPlayerController());

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [PC]()
	{
		if (PC)
		{
			PC->PopUI();
		}
	}, 6.0f, false);
}
