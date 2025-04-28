// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Deconstruct.h"

#include "Kismet/KismetSystemLibrary.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/InteractionComponent.h"
#include "ProjectISG/Core/Character/Player/Component/PlayerHandSlotComponent.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Systems/Grid/Actors/Placement.h"
#include "ProjectISG/Systems/Grid/Manager/GridManager.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"

void UGA_Deconstruct::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo,
                                      const FGameplayAbilityActivationInfo ActivationInfo,
                                      const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		ActorInfo->AvatarActor.Get());

	if (!Player)
	{
		return;
	}
	
	if (!Player->HasAuthority())
	{
		return;
	}

	if (Player->GetHandSlotComponent() && Player->GetInteractionComponent())
	{
		FHitResult TargetResult = Player->GetInteractionComponent()->GetTargetTraceResult();

		AMainPlayerState* PlayerState = Cast<AMainPlayerState>(Player->GetController()->PlayerState);

		if (!PlayerState)
		{
			return;
		}
	
		AGridManager* GridManager = PlayerState->GetGridManager();

		if (!GridManager)
		{
			return;
		}
		
		if (!TargetResult.GetActor())
		{
			return;
		}

		if (APlacement* TargetPlacement = Cast<APlacement>(TargetResult.GetActor()))
		{
			FIntVector GridCoord;
			APlacement* PlacedActor;

			if (GridManager->TryGetPlacement(TargetPlacement, GridCoord, PlacedActor))
			{
				FItemMetaInfo ItemMetaInfo = GridManager->RemovePlacement(GridCoord);
				// UKismetSystemLibrary::PrintString(GetWorld(),  FString::Printf(TEXT("??? %d"), ItemMetaInfo.GetId()));
				PlayerState->GetInventoryComponent()->AddItem(ItemMetaInfo);
			}
		}
	}
}

void UGA_Deconstruct::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility,
	                  bWasCancelled);
}
