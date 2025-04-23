// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Seeding.h"

#include "ProjectISG/Contents/Farming/BaseCrop.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/PlayerInventoryComponent.h"
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

	
	UE_LOG(LogTemp, Warning, TEXT("seeding"));

	const AMainPlayerCharacter* player = Cast<AMainPlayerCharacter>(
		GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (player)
	{
		int id = player->GetPlayerInventoryComponent()->GetCurrentSlotIndex();
		const bool isInteraction = UItemManager::IsItemCanInteraction(
			id);
		if (isInteraction)
		{
			FItemInfoData itemData = UItemManager::GetItemInfoById(
				player->GetPlayerInventoryComponent()->GetCurrentSlotIndex());
			FVector SpawnLocation = player->GetActorLocation();
			SpawnLocation.Z = 0.f;
			FRotator SpawnRotation = FRotator::ZeroRotator;
			ABaseCrop* Crop = GetWorld()->SpawnActor<ABaseCrop>(itemData.GetPlaceItemActor(), SpawnLocation, SpawnRotation);
			if (Crop)
			{
				Crop->SetCropId(id);
			}
			player->GetPlayerInventoryComponent()->RemoveItemCurrentSlotIndex(1);
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
