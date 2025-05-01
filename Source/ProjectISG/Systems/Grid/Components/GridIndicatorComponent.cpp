#include "GridIndicatorComponent.h"

#include "Kismet/GameplayStatics.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Systems/Grid/Actors/Placement.h"
#include "ProjectISG/Systems/Grid/Manager/GridManager.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"

UGridIndicatorComponent::UGridIndicatorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGridIndicatorComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(GetOwner()))
	{
		Player->OnUpdateSelectedItem.AddDynamic(this, &ThisClass::OnUpdateSelectedItem);
	}
}

void UGridIndicatorComponent::OnUpdateSelectedItem(TSubclassOf<AActor> ActorClass, FItemMetaInfo ItemMetaInfo)
{
	const FItemInfoData ItemInfoData = UItemManager::GetItemInfoById(ItemMetaInfo.GetId());

	const bool bIsStructure = ItemInfoData.GetItemType() != EItemType::Equipment && UItemManager::IsItemCanHousing(ItemMetaInfo.GetId());
	
	if (bIsStructure)
	{
		OnActivate(ActorClass);
	}
	else
	{
		OnDeactivate();
	}
}

void UGridIndicatorComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PlayerController || !PlayerController->PlayerState)
	{
		return;
	}
	
	AMainPlayerState* PlayerState = Cast<AMainPlayerState>(PlayerController->PlayerState);

	if (!PlayerState)
	{
		return;
	}
	
	AGridManager* GridManager = PlayerState->GetGridManager();

	if (!GridManager)
	{
		return;
	}

	if (IndicateActor && PlayerController->IsLocalController())
	{
		FVector SnappedLocation = GridManager->GetLocationInPointerDirectionPlacement(
			PlayerController, IndicateActor->GetMeshSize());


		IndicateActor->SetActorLocation(FMath::VInterpTo(IndicateActor->GetActorLocation(), SnappedLocation,
														  0.1f,
														  InterpSpeed));
	}
}

void UGridIndicatorComponent::OnActivate(const TSubclassOf<AActor>& Factory)
{
	if (!PlayerController || !PlayerController->PlayerState)
	{
		return;
	}
	
	AMainPlayerState* PlayerState = Cast<AMainPlayerState>(PlayerController->PlayerState);

	if (!PlayerState)
	{
		return;
	}
	
	AGridManager* GridManager = PlayerState->GetGridManager();

	if (!GridManager)
	{
		return;
	}
	
	AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(GetOwner());
	
	if (Player && Player->IsLocallyControlled())
	{
		SetActive(true);

		if (IndicateActor)
		{
			IndicateActor->Destroy();
		}
	
		AActor* Actor = GetWorld()->SpawnActor<AActor>(Factory);
		IndicateActor = Cast<APlacement>(Actor);

		if (IndicateActor)
		{
			IndicateActor->SetReplicates(false); // 복제 금지
			IndicateActor->SetActorEnableCollision(false); // 충돌 제거
			IndicateActor->SetActorTickEnabled(false);
			IndicateActor->Setup(GridManager->SnapSize);
		}
	}
}

void UGridIndicatorComponent::OnDeactivate()
{
	AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(GetOwner());
	
	if (Player && Player->IsLocallyControlled())
	{
		if (IndicateActor)
		{
			IndicateActor->Destroy();
		}

		SetActive(false);
	}
}

void UGridIndicatorComponent::Execute()
{
}

void UGridIndicatorComponent::Server_Execute_Implementation(FVector Pivot, FVector Location, FRotator Rotation,
                                                            TSubclassOf<APlacement> PlacementClass, FItemMetaInfo_Net ItemMetaInfo)
{
	if (GetOwner()->HasAuthority())
	{
		FItemMetaInfo Info;
		ItemMetaInfo.To(Info);
		ExecuteInternal(Pivot, Location, Rotation, PlacementClass, Info);
	}
}

void UGridIndicatorComponent::ExecuteInternal(FVector Pivot, FVector Location, FRotator Rotation,
	TSubclassOf<APlacement> PlacementClass, FItemMetaInfo ItemMetaInfo)
{
}

