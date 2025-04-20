#include "GridManager.h"

#include "ProjectISG/Systems/Grid/Actors/Placement.h"
#include "ProjectISG/Systems/Grid/Components/GridComponent.h"

AGridManager::AGridManager()
{
	bReplicates = false;
	
	GridComp = CreateDefaultSubobject<UGridComponent>(TEXT("GridComp"));
}

void AGridManager::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority())
	{
		Destroy();
	}
}

void AGridManager::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (GridComp)
	{
		GridComp->OnUpdatedProperties();
	}
}

FVector AGridManager::SnapToGrid(const FVector& Location)
{
	return FVector
	(
		FMath::FloorToInt(Location.X / SnapSize) * SnapSize,
		FMath::FloorToInt(Location.Y / SnapSize) * SnapSize,
		FMath::RoundToInt(Location.Z / SnapSize) * SnapSize
		// FMath::RoundToInt(SnapSize * 0.5f)
	);
}

FVector AGridManager::SnapToGridPlacement(const FVector& Location)
{
	FVector SnappedLocation = SnapToGrid(Location);

	FVector Offset = FVector
	(
		-SnapSize * 0.5f,
		-SnapSize * 0.5f,
		0
	);

	return SnappedLocation + Offset;
}

FIntVector AGridManager::WorldToGridLocation(const FVector& WorldLocation)
{
	return FIntVector
	(
		FMath::FloorToInt(WorldLocation.X / SnapSize),
		FMath::FloorToInt(WorldLocation.Y / SnapSize),
		FMath::RoundToInt(WorldLocation.Z / SnapSize)
		// FMath::RoundToInt(SnapSize * 0.5f)
	);
}

FVector AGridManager::GridToWorldLocation(const FIntVector& GridCoord)
{
	// return FVector(GridCoord.X, GridCoord.Y, 0.5f) * SnapSize;
	return FVector(GridCoord) * SnapSize;
}

FVector AGridManager::GetLocationInFront(AActor* Actor, int32 Distance)
{
	FVector Forward = Actor->GetActorForwardVector();
	FVector Origin = Actor->GetActorLocation();

	FVector RawTarget = Origin + Forward * Distance * SnapSize;
	return SnapToGrid(RawTarget);
}

FVector AGridManager::GetLocationInPointerDirection(APlayerController* PlayerController, int32 Distance)
{
	if (!PlayerController)
	{
		return FVector::ZeroVector;
	}

	FHitResult HitResult;
	if (PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult) && HitResult.
		bBlockingHit)
	{
		return SnapToGrid(HitResult.ImpactPoint);
	}

	return FVector::ZeroVector;
}

FVector AGridManager::GetLocationInPointerDirectionPlacement(APlayerController* PlayerController, FVector MeshSize,
                                                             int32 Distance)
{
	if (!PlayerController)
	{
		return FVector::ZeroVector;
	}

	FHitResult HitResult;
	if (PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult) && HitResult.
		bBlockingHit)
	{
		return SnapToGridPlacement(HitResult.ImpactPoint);
	}

	return FVector::ZeroVector;
}

void AGridManager::RemovePlacement(const FIntVector& GridAt)
{
	if (APlacement* Placement = PlacedMap.FindRef(GridAt))
	{
		if (ReverseMap.Contains(Placement))
		{
			TArray<FIntVector> Arr = ReverseMap.FindRef(Placement);
			for (FIntVector Coord : Arr)
			{
				PlacedMap.Remove(Coord);
			}
			ReverseMap.Remove(Placement);
			Placement->Destroy();
		}
	}
}

bool AGridManager::TryGetPlacement(APlacement* Placement, FIntVector& OutGridAt, APlacement*& OutPlacement)
{
	return TryGetPlacement(Placement->GetActorPivotLocation(), OutGridAt, OutPlacement);
}

bool AGridManager::TryGetPlacement(const FVector& Location, FIntVector& OutGridAt, APlacement*& OutPlacement)
{
	FIntVector ToCoord = WorldToGridLocation(Location);

	if (APlacement* Placement = PlacedMap.FindRef(ToCoord))
	{
		OutGridAt = ToCoord;
		OutPlacement = Placement;

		return true;
	}

	return false;
}

bool AGridManager::TryGetPlacementAt(AActor* Actor, FIntVector& OutGridAt, APlacement*& OutPlacement)
{
	FVector Start = Actor->GetActorLocation();
	FVector End = Start + Actor->GetActorForwardVector() * 1000.f;

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Actor);

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);

	if (bHit)
	{
		FVector HitLocation = HitResult.ImpactPoint;
		return TryGetPlacement(HitLocation, OutGridAt, OutPlacement);
	}

	return false;
}
