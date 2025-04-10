#include "GridManager.h"

#include "ProjectISG/Systems/Grid/Actors/Placement.h"

AGridManager::AGridManager()
{
	//Test..
	ConstructorHelpers::FClassFinder<APlacement> TestPlacement(
		TEXT("/Script/Engine.Blueprint'/Game/BP_TestPlacement.BP_TestPlacement_C'"));

	if (TestPlacement.Succeeded())
	{
		TestPlacementClass = TestPlacement.Class;
	}
}

void AGridManager::BeginPlay()
{
	Super::BeginPlay();

	BuildPlacement(TestPlacementClass, FVector::ZeroVector);
}

FVector AGridManager::SnapToGrid(const FVector& Location)
{
	return FVector
	(
		FMath::RoundToInt(Location.X / SnapSize) * SnapSize,
		FMath::RoundToInt(Location.Y / SnapSize) * SnapSize,
		FMath::RoundToInt(Location.Z / SnapSize) * SnapSize
	);
}

FIntVector AGridManager::WorldToGridLocation(const FVector& WorldLocation)
{
	return FIntVector
	(
		FMath::RoundToInt(WorldLocation.X / SnapSize),
		FMath::RoundToInt(WorldLocation.Y / SnapSize),
		FMath::RoundToInt(WorldLocation.Z / SnapSize)
	);
}

FVector AGridManager::GridToWorldLocation(const FIntVector& GridCoord)
{
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

	// 마우스 위치 -> 월드 방향에서 Deproject
	FVector WorldLocation;
	FVector WorldDirection;

	if (PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		FVector Target = WorldLocation + WorldDirection * Distance * SnapSize;

		return SnapToGrid(Target);
	}

	return FVector::ZeroVector;
}

void AGridManager::RemovePlacement(const FIntVector& GridAt)
{
	if (APlacement* Placement = PlacedMap.FindRef(GridAt))
	{
		Placement->Destroy();
		PlacedMap.Remove(GridAt);
	}
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
