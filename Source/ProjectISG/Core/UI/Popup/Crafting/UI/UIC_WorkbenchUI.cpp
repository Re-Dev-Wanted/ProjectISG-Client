// Fill out your copyright notice in the Description page of Project Settings.

#include "UIC_WorkbenchUI.h"

#include "EnhancedInputComponent.h"

void UUIC_WorkbenchUI::BindInputAction(UEnhancedInputComponent* InputComponent)
{
	Super::BindInputAction(InputComponent);

	InputComponent->BindAction(CloseAction, ETriggerEvent::Started, this, &ThisClass::PopUIFromPlayerController);
}
