// Fill out your copyright notice in the Description page of Project Settings.


#include "UIC_ProductBuyNotification.h"

#include "UIV_ProductBuyNotification.h"
#include "Components/Button.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"

void UUIC_ProductBuyNotification::InitializeController(UBaseUIView* NewView,
	UBaseUIModel* NewModel)
{
	Super::InitializeController(NewView, NewModel);

	const UUIV_ProductBuyNotification* ProductBuyNotification = Cast<
		UUIV_ProductBuyNotification>(GetView());

	ProductBuyNotification->GetBuyButton()->OnClicked.AddDynamic(
		this, &ThisClass::OnClickedBuyButton);
}

void UUIC_ProductBuyNotification::OnClickedBuyButton()
{
	UE_LOG(LogTemp, Warning, TEXT("구매하기"));

	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		GetPlayerController()->GetPawn());
	if (Player)
	{
	}
}
