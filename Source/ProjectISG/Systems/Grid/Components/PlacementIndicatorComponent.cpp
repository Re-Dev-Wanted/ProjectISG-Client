#include "PlacementIndicatorComponent.h"

#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/PlayerInventoryComponent.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Systems/Grid/Actors/Placement.h"
#include "ProjectISG/Systems/Grid/Manager/GridManager.h"

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
	
	if (PlayerInventoryComponent->RemoveItemCurrentSlotIndex(1))
	{
		if (GetOwner()->GetNetMode() == NM_Standalone)
		{
			const TSubclassOf<APlacement> PlacementFactory = IndicateActor->GetClass();
			// UE_LOG(LogTemp, Warning, TEXT("%s"), *GhostPlacement->GetActorLocation().ToCompactString());
			GridManager->BuildPlacementAtGhost(PlacementFactory, IndicateActor);
		}
		else
		{
			if (GetOwner()->HasAuthority())
			{
				const TSubclassOf<APlacement> PlacementFactory = IndicateActor->GetClass();
				// UE_LOG(LogTemp, Warning, TEXT("%s"), *GhostPlacement->GetActorLocation().ToCompactString());
				GridManager->BuildPlacementAtGhost(PlacementFactory, IndicateActor);
			}
			else
			{
				Server_Execute(IndicateActor->GetActorPivotLocation(), IndicateActor->GetActorLocation(), IndicateActor->GetActorRotation(), IndicateActor->GetClass());
			}
		}
		OnDeactivate();
	}
	
}

void UPlacementIndicatorComponent::ExecuteInternal(FVector Pivot, FVector Location, FRotator Rotation,
	TSubclassOf<APlacement> PlacementClass)
{
	Super::ExecuteInternal(Pivot, Location, Rotation, PlacementClass);

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
		GridManager->Server_BuildPlacement(PlacementClass, Pivot, Location, Rotation);
	}
}

void UPlacementIndicatorComponent::Remove()
{
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
	
	if (GridManager)
	{
		// UE_LOG(LogTemp, Warning, TEXT("%s"), *GhostPlacement->GetActorLocation().ToCompactString());
		FIntVector GridCoord;
		APlacement* PlacedActor;

		if (GridManager->TryGetPlacement(IndicateActor, GridCoord, PlacedActor))
		{
			// UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("%s"), *GridCoord.ToString()));
			GridManager->RemovePlacement(GridCoord);
		}
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