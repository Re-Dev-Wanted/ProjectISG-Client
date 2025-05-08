#include "HoedField.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "Net/UnrealNetwork.h"
#include "ProjectISG/Contents/Farming/BaseCrop.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/PlayerHandSlotComponent.h"
#include "ProjectISG/Core/Character/Player/Component/PlayerInventoryComponent.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/GAS/Common/Tag/ISGGameplayTag.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"
#include "ProjectISG/Systems/Inventory/ItemData.h"

AHoedField::AHoedField()
{
	InteractionPos = CreateDefaultSubobject<USceneComponent>(
		TEXT("InteractionPos"));
	InteractionPos->SetupAttachment(Root);
}

void AHoedField::BeginPlay()
{
	Super::BeginPlay();

	MeshComp->SetRenderCustomDepth(false);
}

void AHoedField::GetLifetimeReplicatedProps(
	TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHoedField, IsWet);
	DOREPLIFETIME(AHoedField, PlantedCrop);
}

bool AHoedField::GetCanTouch() const
{
	return !IsWet || !PlantedCrop.IsValid();
}

bool AHoedField::GetCanInteractive() const
{
	return false;
}

FString AHoedField::GetTouchDisplayText() const
{
	return TEXT("");
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

		FString UsingType = UItemManager::GetItemUsingType(ItemMetaInfo.GetId());

		if (UsingType == "Watering")
		{
			FGameplayEventData EventData;
			EventData.EventTag = ISGGameplayTags::Farming_Active_Watering;
			EventData.Instigator = Player;
			EventData.Target = this;
			
			Player->GetAbilitySystemComponent()->HandleGameplayEvent(ISGGameplayTags::Farming_Active_Watering, &EventData);
			return;
		}

		if (UsingType == "Farming" && !PlantedCrop.IsValid())
		{
			FGameplayEventData EventData;
			EventData.EventTag = ISGGameplayTags::Farming_Active_Seeding;
			EventData.Instigator = Player;
			EventData.Target = this;
			
			Player->GetAbilitySystemComponent()->HandleGameplayEvent(ISGGameplayTags::Farming_Active_Seeding, &EventData);

			return;
		}

		uint16 OtherId = UItemManager::GetGeneratedOtherItemIdById(ItemMetaInfo.GetId());

		if (OtherId > 0)
		{
			const FItemInfoData OtherData = UItemManager::GetItemInfoById(OtherId);

			if (OtherData.GetPlaceItemActor() == GetClass())
			{
				if (PlantedCrop.IsValid())
				{
					ABaseCrop* Crop = PlantedCrop.Crop;
					
					if (Crop->GetCurrentState() == ECropState::Mature)
					{
						return;
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

	Crop->OnDryField.AddLambda
	(
		[&]()
		{
			SetWet(false);
		}
	);

	if (IsWet)
	{
		Crop->CropIsGetWater();
	}

	Crop->SetCurrentState(ECropState::Sprout);

	return true;
}

void AHoedField::UpdateState()
{
	UMaterialInstanceDynamic* MatDynamic = MeshComp->CreateAndSetMaterialInstanceDynamic(0);
	MatDynamic->SetScalarParameterValue("Contrast", IsWet? 2.f : 1.f);
	
	if (PlantedCrop.IsValid() && IsWet)
	{
		ABaseCrop* Crop = PlantedCrop.Crop;
		Crop->CropIsGetWater();
	}
}

void AHoedField::OnRep_UpdateState()
{
	UpdateState();
}

void AHoedField::SetWet(bool Watering)
{
	IsWet = Watering;
	
	UpdateState();
}



