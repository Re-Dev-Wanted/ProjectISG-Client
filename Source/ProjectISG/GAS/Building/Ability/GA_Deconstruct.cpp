#include "GA_Deconstruct.h"

#include "AbilitySystemComponent.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/InteractionComponent.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/GAS/Common/Tag/ISGGameplayTag.h"
#include "ProjectISG/Systems/Grid/Actors/Placement.h"
#include "ProjectISG/Systems/Grid/Manager/GridManager.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"

void UGA_Deconstruct::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo*
                                      ActorInfo,
                                      const FGameplayAbilityActivationInfo
                                      ActivationInfo,
                                      const FGameplayEventData*
                                      TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		ActorInfo->AvatarActor.Get());

	if (!Player)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	if (Player->GetHandSlotComponent() && Player->GetInteractionComponent())
	{
		FHitResult TargetResult = Player->GetInteractionComponent()->
		                                  GetTargetTraceResult();

		AMainPlayerState* PlayerState = Cast<AMainPlayerState>(
			Player->GetController()->PlayerState);

		if (!PlayerState)
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
			return;
		}

		AGridManager* GridManager = PlayerState->GetGridManager();

		if (!GridManager)
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
			return;
		}

		if (!TargetResult.GetActor())
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
			return;
		}

		if (APlacement* TargetPlacement = Cast<APlacement>(
			TargetResult.GetActor()))
		{
			FIntVector GridCoord;
			uint16 ItemId = 0;

			if (GridManager->
				TryGetPlacement(TargetPlacement, GridCoord, ItemId))
			{
				GridManager->Server_RemovePlacement(GridCoord, ItemId);

				if (Player->IsLocallyControlled())
				{
					// 인벤토리는 각자의 것이므로 로컬에서만 작동하게 한다.
					if (ItemId > 0)
					{
						const FString UsingType =
							UItemManager::GetItemUsingType(ItemId);

						if (UsingType != "Disposability")
						{
							FItemMetaInfo ItemMetaInfo;
							ItemMetaInfo.SetId(ItemId);
							ItemMetaInfo.SetCurrentCount(1);
							PlayerState->GetInventoryComponent()->AddItem(
								ItemMetaInfo);
						}
					}
				}


				FGameplayCueParameters Param;
				Param.AbilityLevel = 1;
				Param.EffectCauser = TargetPlacement;
				Player->GetAbilitySystemComponent()->ExecuteGameplayCue(
					TargetPlacement->OnRemoveCueTag, Param);

				TargetPlacement->Destroy();
			}
		}

		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}

void UGA_Deconstruct::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                 const FGameplayAbilityActorInfo* ActorInfo,
                                 const FGameplayAbilityActivationInfo
                                 ActivationInfo,
                                 bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility,
	                  bWasCancelled);
}
