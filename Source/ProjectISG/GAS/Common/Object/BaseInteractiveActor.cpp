#include "BaseInteractiveActor.h"

#include "Net/UnrealNetwork.h"

void ABaseInteractiveActor::GetLifetimeReplicatedProps(
	TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseInteractiveActor, UsingCharacter)
}

void ABaseInteractiveActor::SetUsingOwner(AMainPlayerCharacter* NewOwner)
{
	if (HasAuthority())
	{
		SetUsingCharacter(NewOwner);
	}
	else
	{
		Server_SetUsingOwner(NewOwner);
	}
}

void ABaseInteractiveActor::Server_SetUsingOwner_Implementation(
	AMainPlayerCharacter* NewOwner)
{
	SetUsingCharacter(NewOwner);
}
