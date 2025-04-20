#include "GridManager.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "ProjectISG/Systems/Grid/Actors/Placement.h"
#include "ProjectISG/Systems/Grid/Components/GridComponent.h"

AGridManager::AGridManager()
{
	bReplicates = true;
	bAlwaysRelevant = true;
	bNetLoadOnClient = true;
	SetReplicates(true);
	
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

void AGridManager::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGridManager, PlacementGridContainer);
}

void AGridManager::OnRep_AddPlacement()
{
	// PlacedMap.Empty();
	//
	// for (const FPlacementGridEntry& Info : PlacementRepInfos)
	// {
	// 	if (Info.IsValid())
	// 	{
	// 		PlacedMap.Add(Info.GridCoord, Info.Placement.Get());
	// 	}
	// }
	//
	// UKismetSystemLibrary::PrintString(GetWorld(),FString::Printf(TEXT("OnRep_PlacementGrid() called on %d"), PlacedMap.Num()));
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
	FIntVector Grid = WorldToGridLocation(Location);
	return GridToWorldLocation(Grid);
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
	return FVector(
		GridCoord.X * SnapSize + SnapSize * 0.5f,
		GridCoord.Y * SnapSize + SnapSize * 0.5f,
		GridCoord.Z * SnapSize                    // 보통 Z는 그대로
	);
	
	// return FVector(GridCoord) * SnapSize;
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
	TWeakObjectPtr<APlacement> Placement = PlacementGridContainer.PlacedMap.FindRef(GridAt);
	if (Placement.IsValid())
	{
		// 모든 관련 좌표 제거
		TArray<int32> IndicesToRemove;

		for (int32 i = 0; i < PlacementGridContainer.Items.Num(); ++i)
		{
			const FPlacementGridEntry& Entry = PlacementGridContainer.Items[i];

			if (Entry.Placement == Placement)
			{
				IndicesToRemove.Add(i);
			}
		}

		// 실제 배열에서 제거
		for (int32 Index : IndicesToRemove)
		{
			PlacementGridContainer.Items.RemoveAt(Index);
			PlacementGridContainer.MarkArrayDirty();
		}

		// 캐시 Map도 정리
		for (auto It = PlacementGridContainer.PlacedMap.CreateIterator(); It; ++It)
		{
			if (It.Value() == Placement)
			{
				It.RemoveCurrent();
			}
		}

		// 가구 제거
		Placement->Destroy();
	}
}

bool AGridManager::TryGetPlacement(APlacement* Placement, FIntVector& OutGridAt, APlacement*& OutPlacement)
{
	return TryGetPlacement(Placement->GetActorPivotLocation(), OutGridAt, OutPlacement);
}

bool AGridManager::TryGetPlacement(const FVector& Location, FIntVector& OutGridAt, APlacement*& OutPlacement)
{
	FIntVector ToCoord = WorldToGridLocation(Location);

	if (PlacementGridContainer.PlacedMap.Contains(ToCoord))
	{
		TWeakObjectPtr<APlacement> Placement = PlacementGridContainer.PlacedMap[ToCoord];
		if (Placement.IsValid())
		{
			OutGridAt = ToCoord;
			OutPlacement = Placement.Get();

			return true;
		}
	}

	// if (APlacement* Placement = PlacedMap.FindRef(ToCoord))
	// {
	// 	OutGridAt = ToCoord;
	// 	OutPlacement = Placement;
	//
	// 	return true;
	// }

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

void AGridManager::Server_BuildPlacement_Implementation(TSubclassOf<APlacement> PlacementClass, FVector Pivot,
	FVector Location, FRotator Rotation)
{
	BuildPlacement(PlacementClass, Pivot, Location, Rotation);
}
