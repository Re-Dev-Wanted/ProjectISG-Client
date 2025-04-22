// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Seeding.h"

#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"

void UGA_Seeding::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                  const FGameplayAbilityActorInfo* ActorInfo,
                                  const FGameplayAbilityActivationInfo
                                  ActivationInfo,
                                  const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	UE_LOG(LogTemp, Warning, TEXT("log"));

	const AMainPlayerCharacter* player = Cast<AMainPlayerCharacter>(
		GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (player)
	{
		if (UItemManager::IsItemCanInteraction(player->GetMainHandItemId()))
		{
			UE_LOG(LogTemp, Warning, TEXT("work"));
			FItemInfoData itemData = UItemManager::GetItemInfoById(
				player->GetMainHandItemId());
			GetWorld()->SpawnActor(itemData.GetPlaceItemActor());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("doesn't work"));
	}
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
