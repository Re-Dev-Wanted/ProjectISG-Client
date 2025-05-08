#include "SeatingFurniture.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/InteractionComponent.h"
#include "ProjectISG/GAS/Common/Tag/ISGGameplayTag.h"

void ASeatingFurniture::OnInteractive(AActor* Causer)
{
	Super::OnInteractive(Causer);

	if (AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(Causer))
	{
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
	return bInteractive;
}

FString ASeatingFurniture::GetDisplayText() const
{
	return TEXT("앉기");
}

FVector ASeatingFurniture::GetStartInteractPoint() const
{
	if (MeshComp && MeshComp->DoesSocketExist(TEXT("SitSocket")))
	{
		return MeshComp->GetSocketLocation(TEXT("SitSocket"));
	}

	return Super::GetStartInteractPoint();
}

