#include "GA_Seeding.h"

#include "ProjectISG/Contents/Building/Props/HoedField.h"
#include "ProjectISG/Contents/Farming/BaseCrop.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/PlayerInventoryComponent.h"
#include "ProjectISG/GAS/Common/Ability/Utility/PlayMontageWithEvent.h"
#include "ProjectISG/GAS/Common/Tag/ISGGameplayTag.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"
#include "ProjectISG/Systems/Logging/LoggingEnum.h"
#include "ProjectISG/Systems/Logging/LoggingStruct.h"
#include "ProjectISG/Systems/Logging/LoggingSubSystem.h"
#include "ProjectISG/Utils/EnumUtil.h"

void UGA_Seeding::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                  const FGameplayAbilityActorInfo* ActorInfo,
                                  const FGameplayAbilityActivationInfo
                                  ActivationInfo,
                                  const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		ActorInfo->AvatarActor.Get());
	if (!IsValid(Player))
	{
		return;
	}

	int id = Player->GetPlayerInventoryComponent()->GetCurrentSlotIndex();
	const bool isInteraction = UItemManager::IsItemCanInteraction(
		id);

	UE_LOG(LogTemp, Warning, TEXT("씨앗 심기 check, %d"),
	       TriggerEventData->Target == nullptr);

	if (isInteraction)
	{
		if (Player)
		{
			

			LoggingToSeeding();
		}

		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo,
		           true,
		           false);
	}

	// if (isInteraction)
	// {
	// 	AT_SeedingAnim = UPlayMontageWithEvent::InitialEvent(
	// 		this, NAME_None,
	// 		Player->GetSeedingMontage(),
	// 		FGameplayTagContainer(),
	// 		*TriggerEventData
	// 	);
	//
	// 	AT_SeedingAnim->Activate();
	// 	BlockInputForMontage(true);
	// 	AT_SeedingAnim->OnCompleted.AddDynamic(this, &ThisClass::CreateSeed);
	// }
	// else
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("상호작용 아이템 아님"));
	// 	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	// }
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

void UGA_Seeding::CreateSeed(FGameplayTag EventTag,
                             FGameplayEventData EventData)
{
	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		CurrentActorInfo->AvatarActor.Get());

	if (Player)
	{
		// const uint16 ItemId = Player->GetPlayerInventoryComponent()->
		//                               GetCurrentSlotIndex();
		// FItemInfoData itemData = UItemManager::GetItemInfoById(ItemId);
		//
		// const AActor* Target = EventData.Target.Get();
		// const AHoedField* ConstField = Cast<AHoedField>(Target);
		//
		// AHoedField* HoedField = const_cast<AHoedField*>(ConstField);
		//
		// if (HoedField->PlantCrop(itemData, ItemId))
		// {
		// 	Player->GetPlayerInventoryComponent()->
		// 	        RemoveItemCurrentSlotIndex(1);
		// }

		BlockInputForMontage(false);
		LoggingToSeeding();
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true,
	           false);
}

void UGA_Seeding::LoggingToSeeding()
{
	FDiaryLogParams LogParams;
	LogParams.Location = TEXT("농장");
	LogParams.ActionType = ELoggingActionType::FARMING;
	LogParams.ActionName = ELoggingActionName::plant_crop;

	GetWorld()->GetGameInstance()->GetSubsystem<ULoggingSubSystem>()->LoggingData(LogParams);

}
