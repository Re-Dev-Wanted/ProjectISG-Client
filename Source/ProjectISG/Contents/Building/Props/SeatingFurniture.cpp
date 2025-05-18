#include "SeatingFurniture.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/InteractionComponent.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/GAS/Common/Tag/ISGGameplayTag.h"

void ASeatingFurniture::OnInteractive(AActor* Causer)
{
	Super::OnInteractive(Causer);

	if (AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(Causer))
	{
		AMainPlayerController* PC = Cast<AMainPlayerController>(
			Player->GetController());

		if (!PC)
		{
			return;
		}

		SetInteractingPlayer(Player);
		
		if (HasAuthority())
		{
			SetOwner(PC);
		}
		else
		{
			PC->Server_SetOwnerActor(this);

			Player->GetInteractionComponent()->Server_OnInteractiveResponse(Causer);
		}
		
		Player->GetInteractionComponent()->SetIsInteractive(false);

		FGameplayEventData EventData;
		EventData.EventTag = ISGGameplayTags::Building_Active_StartSitDown;
		EventData.Instigator = Player;
		EventData.Target = this;
		
		Player->GetAbilitySystemComponent()->HandleGameplayEvent(ISGGameplayTags::Building_Active_StartSitDown, &EventData);
	}
}

bool ASeatingFurniture::GetCanInteractive() const
{
	return !IsValid(GetInteractingPlayer());
}

void ASeatingFurniture::OnInteractiveResponse(AActor* Causer)
{
	Super::OnInteractiveResponse(Causer);

	AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(Causer);
	Server_SetInteractingPlayer(Player);
}

FString ASeatingFurniture::GetInteractiveDisplayText() const
{
	return TEXT("앉기");
}

void ASeatingFurniture::OnClosed()
{
	if (!IsValid(GetInteractingPlayer()))
	{
		return;
	}
	
	AMainPlayerController* PC = Cast<AMainPlayerController>(
		GetInteractingPlayer()->GetController());

	if (!PC)
	{
		return;
	}
	
	if (!HasAuthority())
	{
		GetInteractingPlayer()->GetInteractionComponent()->Server_OnInteractiveResponse
		(nullptr);
	}

	SetInteractingPlayer(nullptr);
}
