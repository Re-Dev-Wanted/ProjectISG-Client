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

		FGameplayTagContainer ActivateTag;
		ActivateTag.AddTag(ISGGameplayTags::Building_Active_StartSitDown);
		Player->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(ActivateTag);
	}
}

bool ASeatingFurniture::GetCanInteractive() const
{
	return true;
}

FString ASeatingFurniture::GetDisplayText() const
{
	return TEXT("앉기");
}

