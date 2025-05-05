#include "FishingSpotField.h"

#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Components/BoxComponent.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/PlayerHandSlotComponent.h"
#include "ProjectISG/GAS/Common/Tag/ISGGameplayTag.h"

AFishingSpotField::AFishingSpotField()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Root = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Root->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Block);
}

bool AFishingSpotField::GetCanTouch() const
{
	return true;
}

FString AFishingSpotField::GetDisplayText() const
{
	return TEXT("낚시하기");
}

void AFishingSpotField::OnTouch(AActor* Causer)
{
	IInteractionInterface::OnTouch(Causer);

	if (AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(Causer))
	{
		FString HandItemUsingType = Player->GetHandSlotComponent()->GetItemUsingType();

		if (HandItemUsingType == "Fishing")
		{
			FGameplayEventData EventData;
			EventData.EventTag = ISGGameplayTags::Fishing_Active_CastBobber;
			EventData.Instigator = Player;
			EventData.Target = this;
			
			Player->GetAbilitySystemComponent()->HandleGameplayEvent(ISGGameplayTags::Fishing_Active_CastBobber, &EventData);
		}
	}
}

void AFishingSpotField::OnTouchResponse()
{
	IInteractionInterface::OnTouchResponse();
}
