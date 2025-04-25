#include "GA_Seeding.h"

#include "ProjectISG/Contents/Farming/BaseCrop.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/PlayerInventoryComponent.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"
#include "ProjectISG/Utils/EnumUtil.h"

void UGA_Seeding::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                  const FGameplayAbilityActorInfo* ActorInfo,
                                  const FGameplayAbilityActivationInfo
                                  ActivationInfo,
                                  const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);


	const AMainPlayerCharacter* player = Cast<AMainPlayerCharacter>(
		ActorInfo->AvatarActor.Get());

	UE_LOG(LogTemp, Warning, TEXT("씨앗 심기, %s"),
	       *FEnumUtil::GetClassEnumKeyAsString(player->GetLocalRole()));

	if (player)
	{
		if (player->HasAuthority() == false)
		{
			return;
		}
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
			ABaseCrop* Crop = GetWorld()->SpawnActor<ABaseCrop>(
				itemData.GetPlaceItemActor(), SpawnLocation, SpawnRotation);
			if (Crop)
			{
				Crop->SetCropId(id);
			}
			player->GetPlayerInventoryComponent()->
			        RemoveItemCurrentSlotIndex(1);
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
