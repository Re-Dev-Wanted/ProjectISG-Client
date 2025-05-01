#include "HoedField.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "ProjectISG/Contents/Farming/BaseCrop.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/PlayerHandSlotComponent.h"
#include "ProjectISG/Core/Character/Player/Component/PlayerInventoryComponent.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/GAS/Common/Tag/ISGGameplayTag.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"

void AHoedField::BeginPlay()
{
	Super::BeginPlay();

	MeshComp->SetRenderCustomDepth(false);
}

// 경작된 땅은 해머로 파괴되어선 안된다
// 만약 작물이 있다면 작물 체크
	// ↳작물이 다 자랐다면 파밍
	// ↳아니라면 씨앗을 드롭하고 빈 경작된 땅으로 전환
// 빈 경작된 땅이라면 철거
void AHoedField::OnTouch(AActor* Causer)
{
	ABaseInteractiveActor::OnTouch(Causer);

	if (AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(Causer))
	{
		int SlotIndex = Player->GetPlayerInventoryComponent()->GetCurrentSlotIndex();

		const AMainPlayerState* PS = Player->GetPlayerState<AMainPlayerState>();

		const FItemMetaInfo ItemMetaInfo = PS->GetInventoryComponent()->GetInventoryList()[SlotIndex];

		uint16 OtherId = UItemManager::GetGeneratedOtherItemIdById(ItemMetaInfo.GetId());

		FString UsingType = UItemManager::GetItemUsingType(ItemMetaInfo.GetId());

		if (UsingType == "Farming" && !PlantedCrop.IsValid())
		{
			FGameplayEventData EventData;
			EventData.EventTag = ISGGameplayTags::Farming_Active_Seeding;
			EventData.Instigator = Player;
			EventData.Target = this;
			
			Player->GetAbilitySystemComponent()->HandleGameplayEvent(ISGGameplayTags::Farming_Active_Seeding, &EventData);

			return;
		}

		if (OtherId > 0)
		{
			const FItemInfoData OtherData = UItemManager::GetItemInfoById(OtherId);

			if (OtherData.GetPlaceItemActor() == GetClass())
			{
				if (PlantedCrop.IsValid())
				{
					ABaseCrop* Crop = PlantedCrop.Crop.Get();
					
					if (Crop->GetbIsMature())
					{
						Crop->OnInteractive(Player);
					}
					else
					{
						// 작물 제거, 해당 작물에 해당하는 씨앗 뱉기, 인벤토리에 넣기
						FItemMetaInfo SeedMetaInfo = UItemManager::GetInitialItemMetaDataById(PlantedCrop.CropId);
						PS->GetInventoryComponent()->AddItem(SeedMetaInfo);
					}
					
					PlantedCrop.Clear(true);
				}
				else
				{
					FGameplayTagContainer ActivateTag;
					ActivateTag.AddTag(ISGGameplayTags::Building_Active_Deconstruct);
					Player->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(ActivateTag);
				}
			}
		}
	
	}
}

bool AHoedField::PlantCrop(FItemInfoData CropData, uint16 CropId)
{
	if (PlantedCrop.IsValid())
	{
		return false;
	}
	
	ABaseCrop* Crop = GetWorld()->SpawnActor<ABaseCrop>
	(
		CropData.GetPlaceItemActor(),
		GetActorLocation(),
		FRotator::ZeroRotator
	);

	PlantedCrop.Crop = Crop;
	PlantedCrop.CropId = CropId;

	return true;
}


