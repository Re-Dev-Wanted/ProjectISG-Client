// Fill out your copyright notice in the Description page of Project Settings.

#include "GA_Construct.h"

#include "Kismet/KismetSystemLibrary.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/PlayerInventoryComponent.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Systems/Grid/Components/PlacementIndicatorComponent.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"

void UGA_Construct::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(ActorInfo->AvatarActor.Get());

	if (!IsValid(Player))
	{
		return;
	}

	if (!Player->HasAuthority())
	{
		return;
	}
	

	int Index = Player->GetPlayerInventoryComponent()->GetCurrentSlotIndex();

	const AMainPlayerState* PS = Cast<AMainPlayerState>(Player->GetPlayerState());

	const uint16 ItemId = PS->GetInventoryComponent()->GetInventoryList()[
		Index].GetId();

	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("construct %d"), ItemId));

	const bool bIsHousing = UItemManager::IsItemCanHousing(ItemId);
	
	if (bIsHousing)
	{
		Player->GetPlacementIndicatorComponent()->Execute();
	}
}

void UGA_Construct::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
