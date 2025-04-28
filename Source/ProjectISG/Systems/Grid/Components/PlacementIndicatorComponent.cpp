#include "PlacementIndicatorComponent.h"

#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/PlayerInventoryComponent.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Systems/Grid/Actors/Placement.h"
#include "ProjectISG/Systems/Grid/Manager/GridManager.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"

class AMainPlayerCharacter;

void UPlacementIndicatorComponent::TickComponent(float DeltaTime, ELevelTick TickType,
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
		
		IndicateActor->SetActorRotation(FMath::RInterpTo(IndicateActor->GetActorRotation(),
		                                                  FRotator(0, GetDegrees(RotateDirection), 0), 0.1f,
		                                                  InterpSpeed));
		FIntVector GridCoord;
		APlacement* PlacedActor;

		bool bIsBlock = GridManager->TryGetPlacement(SnappedLocation, GridCoord, PlacedActor);

		IndicateActor->SetColor(true, bIsBlock);
	}
}

void UPlacementIndicatorComponent::Execute()
{
	Super::Execute();

	if (!IndicateActor)
	{
		return;
	}
	
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
	
	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(GetOwner());

	const TObjectPtr<UPlayerInventoryComponent> PlayerInventoryComponent = Player->GetPlayerInventoryComponent();

	const int SlotIndex = PlayerInventoryComponent->GetCurrentSlotIndex();

	FItemMetaInfo ItemMetaInfo = PlayerState->GetInventoryComponent()->GetInventoryList()[SlotIndex];
	
	if (PlayerInventoryComponent->RemoveItemCurrentSlotIndex(1))
	{
		if (GetOwner()->GetNetMode() == NM_Standalone)
		{
			const TSubclassOf<APlacement> PlacementFactory = IndicateActor->GetClass();
			// UE_LOG(LogTemp, Warning, TEXT("%s"), *GhostPlacement->GetActorLocation().ToCompactString());
			GridManager->BuildPlacementAtGhost(PlacementFactory, ItemMetaInfo, IndicateActor);
		}
		else
		{
			if (GetOwner()->HasAuthority())
			{
				const TSubclassOf<APlacement> PlacementFactory = IndicateActor->GetClass();
				// UE_LOG(LogTemp, Warning, TEXT("%s"), *GhostPlacement->GetActorLocation().ToCompactString());
				GridManager->BuildPlacementAtGhost(PlacementFactory, ItemMetaInfo, IndicateActor);
			}
			else
			{
				FItemMetaInfo_Net Info(ItemMetaInfo);
				Server_Execute(IndicateActor->GetActorPivotLocation(), IndicateActor->GetActorLocation(), IndicateActor->GetActorRotation(), IndicateActor->GetClass(), Info);
			}
		}
		OnDeactivate();
	}
	
}

void UPlacementIndicatorComponent::ExecuteInternal(FVector Pivot, FVector Location, FRotator Rotation,
	TSubclassOf<APlacement> PlacementClass, FItemMetaInfo ItemMetaInfo)
{
	Super::ExecuteInternal(Pivot, Location, Rotation, PlacementClass, ItemMetaInfo);

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

	if (GridManager)
	{
		FItemMetaInfo_Net Info(ItemMetaInfo);
		GridManager->Server_BuildPlacement(PlacementClass, Info, Pivot, Location, Rotation);
	}
}

void UPlacementIndicatorComponent::Rotate(bool bClockwise)
{
	if (bClockwise)
	{
		RotateDirection = RotateDirection << static_cast<uint8>(1);
		return;
	}
	RotateDirection = RotateDirection >> static_cast<uint8>(1);
}