// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseInteractiveActor.h"

#include "Net/UnrealNetwork.h"

void ABaseInteractiveActor::GetLifetimeReplicatedProps(
	TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InteractingPlayer);
}
