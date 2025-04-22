// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Seeding.h"

#include "ProjectISG/Contents/Farming/BaseCrop.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"

void UGA_Seeding::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                  const FGameplayAbilityActorInfo* ActorInfo,
                                  const FGameplayAbilityActivationInfo
                                  ActivationInfo,
                                  const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const AMainPlayerCharacter* player = Cast<AMainPlayerCharacter>(
		GetWorld()->GetFirstPlayerController()->GetCharacter());
	const AMainPlayerState* ps = player->GetPlayerState<AMainPlayerState>();
	if (player && ps)
	{
		const bool isInteraction = UItemManager::IsItemCanInteraction(
			player->GetMainHandItemId());
		if (isInteraction)
		{
			FItemInfoData itemData = UItemManager::GetItemInfoById(
				player->GetMainHandItemId());
			FVector SpawnLocation = player->GetActorLocation();
			FRotator SpawnRotation = FRotator::ZeroRotator;
			GetWorld()->SpawnActor<ABaseCrop>(itemData.GetPlaceItemActor(),
			                                  SpawnLocation, SpawnRotation);
			ps->GetInventoryComponent()->RemoveItem(
				player->GetMainHandItemId(), 1);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("doesn't work"));
	}


	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UGA_Seeding::EndAbility(const FGameplayAbilitySpecHandle Handle,
                             const FGameplayAbilityActorInfo* ActorInfo,
                             const FGameplayAbilityActivationInfo
                             ActivationInfo,
                             bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility,
	                  bWasCancelled);
}
