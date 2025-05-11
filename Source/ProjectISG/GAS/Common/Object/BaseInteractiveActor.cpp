#include "BaseInteractiveActor.h"

#include "Net/UnrealNetwork.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"

void ABaseInteractiveActor::GetLifetimeReplicatedProps(
	TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseInteractiveActor, UsingCharacter)
}

void ABaseInteractiveActor::SetUsingOwner(AActor* NewOwner)
{
	if (HasAuthority())
	{
		SetUsingOwner_Internal(NewOwner);
	}
	else
	{
		SetUsingOwner_Internal(NewOwner);
		Server_SetUsingOwner(NewOwner);
	}
}

void ABaseInteractiveActor::Server_SetUsingOwner_Implementation(
	AActor* NewOwner)
{
	SetUsingOwner_Internal(NewOwner);
}

void ABaseInteractiveActor::SetUsingOwner_Internal(AActor* NewOwner)
{
	SetOwner(NewOwner);
	if (AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(NewOwner))
	{
		SetUsingCharacter(Player);
	}
}
