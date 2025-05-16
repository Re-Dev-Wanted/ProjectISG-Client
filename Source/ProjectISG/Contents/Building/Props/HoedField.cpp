#include "HoedField.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "ProjectISG/Contents/Farming/BaseCrop.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/InteractionComponent.h"
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

void AHoedField::OnSpawned()
{
	//TODO:: 테스트 (20%)
	float Percent = FMath::RandRange(1.f, 100.f);

	if (Percent > ChanceBasedPercent)
	{
		return;
	}

	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		GetWorld()->GetFirstPlayerController()->GetPawn());

	if (!Player)
	{
		return;
	}
	
	// UKismetSystemLibrary::PrintString(GetWorld(), TEXT("돌맹이 나올것이여"));

	int SlotIndex = Player->GetPlayerInventoryComponent()->GetCurrentSlotIndex();
	
	const AMainPlayerState* PS = Player->GetPlayerState<AMainPlayerState>();

	const FItemMetaInfo ItemMetaInfo = PS->GetInventoryComponent()->
										   GetInventoryList()[SlotIndex];

	uint16 ItemId = UItemManager::GetChanceBasedSpawnItemIdById(ItemMetaInfo.GetId
	());

	if (ItemId > 0)
	{
		const FItemMetaInfo AddItemMetaInfo = 
		UItemManager::GetInitialItemMetaDataById
		(ItemId);

		PS->GetInventoryComponent()->AddItem(AddItemMetaInfo);
	}
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
	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		GetWorld()->GetFirstPlayerController()->GetPawn());

	if (!Player)
	{
		return false;
	}

	const FString HandItemUsingType = Player->GetHandSlotComponent()
	                                        ->GetItemUsingType();

	if (HandItemUsingType == "Farming")
	{
		return !PlantedCrop.IsValid();
	}

	if (HandItemUsingType == "Watering")
	{
		return !IsWet && PlantedCrop.IsValid() && PlantedCrop.Crop->
			GetCurrentState() != ECropState::Mature;
	}

	int SlotIndex = Player->GetPlayerInventoryComponent()->
	                        GetCurrentSlotIndex();

	const AMainPlayerState* PS = Player->GetPlayerState<AMainPlayerState>();

	const FItemMetaInfo ItemMetaInfo = PS->GetInventoryComponent()->
	                                       GetInventoryList()[SlotIndex];

	uint16 OtherId = UItemManager::GetGeneratedOtherItemIdById(
		ItemMetaInfo.GetId());

	if (OtherId > 0)
	{
		return true;
	}

	return false;
}

bool AHoedField::GetCanInteractive() const
{
	return false;
}

FString AHoedField::GetTouchDisplayText(AActor* Causer) const
{
	if (AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(Causer))
	{
		int SlotIndex = Player->GetPlayerInventoryComponent()->
		                        GetCurrentSlotIndex();

		const AMainPlayerState* PS = Player->GetPlayerState<AMainPlayerState>();

		const FItemMetaInfo ItemMetaInfo = PS->GetInventoryComponent()->
		                                       GetInventoryList()[SlotIndex];

		const uint16 ItemId = Player->GetHandSlotComponent()->GetItemId();
		FItemInfoData itemData = UItemManager::GetItemInfoById(ItemId);
		FString UsingType = UItemManager::GetItemUsingType(ItemId);

		if (UsingType == "Watering" && PlantedCrop.IsValid())
		{
			return TEXT("물주기");
		}

		if (UsingType == "Farming" && !PlantedCrop.IsValid())
		{
			return FString::Printf(TEXT("%s 심기"), *itemData.GetDisplayName());
		}

		uint16 OtherId = UItemManager::GetGeneratedOtherItemIdById(
			ItemMetaInfo.GetId());

		if (OtherId > 0)
		{
			const FItemInfoData OtherData = UItemManager::GetItemInfoById(
				OtherId);

			if (OtherData.GetPlaceItemActor() == GetClass())
			{
				if (PlantedCrop.IsValid())
				{
					ABaseCrop* Crop = PlantedCrop.Crop;
					FItemInfoData SeedInfoData = UItemManager::GetItemInfoById(
						PlantedCrop.CropId);

					if (Crop->GetCurrentState() != ECropState::Mature)
					{
						return FString::Printf(
							TEXT("%s 회수하기"), *SeedInfoData.GetDisplayName());
					}
				}
				else
				{
					return TEXT("밭 제거");
				}
			}
		}
	}

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
		int SlotIndex = Player->GetPlayerInventoryComponent()->
		                        GetCurrentSlotIndex();

		const AMainPlayerState* PS = Player->GetPlayerState<AMainPlayerState>();

		const FItemMetaInfo ItemMetaInfo = PS->GetInventoryComponent()->
		                                       GetInventoryList()[SlotIndex];

		//FString UsingType = UItemManager::GetItemUsingType(ItemMetaInfo.GetId());

		const uint16 ItemId = Player->GetHandSlotComponent()->GetItemId();
		const FItemInfoData ItemData = UItemManager::GetItemInfoById(ItemId);
		const FString UsingType = UItemManager::GetItemUsingType(ItemId);

		if (UsingType == "Watering" && PlantedCrop.IsValid())
		{
			FGameplayEventData EventData;
			EventData.EventTag = ISGGameplayTags::Farming_Active_Watering;
			EventData.Instigator = Player;
			EventData.Target = this;

			Player->GetAbilitySystemComponent()->HandleGameplayEvent(
				ISGGameplayTags::Farming_Active_Watering, &EventData);

			Player->GetInteractionComponent()->Server_OnTouchResponse(Causer);
			return;
		}

		if (UsingType == "Farming" && !PlantedCrop.IsValid())
		{
			FGameplayEventData EventData;
			EventData.EventTag = ISGGameplayTags::Farming_Active_Seeding;
			EventData.Instigator = Player;
			EventData.Target = this;

			Player->GetAbilitySystemComponent()->HandleGameplayEvent(
				ISGGameplayTags::Farming_Active_Seeding, &EventData);

			Player->GetInteractionComponent()->Server_OnTouchResponse(Causer);
			Player->GetPlayerInventoryComponent()->
			        RemoveItemCurrentSlotIndex(1);

			return;
		}

		uint16 OtherId = UItemManager::GetGeneratedOtherItemIdById(
			ItemMetaInfo.GetId());

		if (OtherId > 0)
		{
			const FItemInfoData OtherData = UItemManager::GetItemInfoById(
				OtherId);

			if (OtherData.GetPlaceItemActor() == GetClass())
			{
				if (PlantedCrop.IsValid())
				{
					ABaseCrop* Crop = PlantedCrop.Crop;

					if (Crop->GetCurrentState() == ECropState::Mature)
					{
						return;
					}
					if (Player->IsLocallyControlled())
					{
						// 작물 제거, 해당 작물에 해당하는 씨앗 뱉기, 인벤토리에 넣기
						FItemMetaInfo SeedMetaInfo =
							UItemManager::GetInitialItemMetaDataById(
								PlantedCrop.CropId);
						PS->GetInventoryComponent()->AddItem(SeedMetaInfo);
					}

					PlantedCrop.Clear(true);
				}
				else
				{
					FGameplayTagContainer ActivateTag;
					ActivateTag.AddTag(
						ISGGameplayTags::Building_Active_Deconstruct);
					Player->GetAbilitySystemComponent()->
					        TryActivateAbilitiesByTag(ActivateTag);
				}
			}
		}
	}
}

void AHoedField::OnTouchResponse(AActor* Causer)
{
	Super::OnTouchResponse(Causer);
	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(Causer);
	const uint16 ItemId = Player->GetHandSlotComponent()->GetItemId();

	const FItemInfoData ItemData = UItemManager::GetItemInfoById(ItemId);
	const FString UsingType = UItemManager::GetItemUsingType(ItemId);

	UE_LOG(LogTemp, Warning, TEXT("서버실행 슬롯 아이템 타입 %s"), *UsingType);

	if (UsingType == "Farming" && !PlantedCrop.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("씨앗심기 (서버)"));
		PlantCrop(ItemData, ItemId);
	}

	if (UsingType == "Watering" && PlantedCrop.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("물주기 (서버)"));
		SetWet(true);
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

	Crop->HarvestCrop.AddLambda
	(
		[this]()
		{
			PlantedCrop.Clear(true);
		}
	);

	if (IsWet)
	{
		Crop->CropIsGetWater();
	}

	Crop->SetCurrentState(ECropState::Seedling);

	return true;
}

void AHoedField::UpdateState()
{
	UMaterialInstanceDynamic* MatDynamic = MeshComp->
		CreateAndSetMaterialInstanceDynamic(0);
	MatDynamic->SetScalarParameterValue("Contrast", IsWet ? 2.f : 1.f);

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

void AHoedField::SetWet_Implementation(bool Watering)
{
	IsWet = Watering;

	UpdateState();
}
