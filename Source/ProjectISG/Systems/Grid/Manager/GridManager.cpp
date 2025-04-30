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
	
	GridComp = CreateDefaultSubobject<UGridComponent>(TEXT("GridComp"));
}

void AGridManager::BeginPlay()
{
	Super::BeginPlay();

	PlacementGridContainer.SetOwner(this);

	SetVisibleGrid(false);

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

FVector AGridManager::SnapToGrid(const FVector& Location)
{
	return FVector
	(
		FMath::FloorToInt(Location.X / SnapSize) * SnapSize,
		FMath::FloorToInt(Location.Y / SnapSize) * SnapSize,
		FMath::RoundToInt(Location.Z / SnapSize) * SnapSize
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
	);
}

FVector AGridManager::GridToWorldLocation(const FIntVector& GridCoord)
{
	return FVector
	(
		GridCoord.X * SnapSize + SnapSize * 0.5f,
		GridCoord.Y * SnapSize + SnapSize * 0.5f,
		GridCoord.Z * SnapSize                    // 보통 Z는 그대로
	);
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

void AGridManager::BuildPlacement(TSubclassOf<APlacement> PlacementClass,
	uint16 ItemId, const FVector& Pivot, const FVector& Location,
	const FRotator& Rotation)
{
	FIntVector GridCoord = FIntVector
	(
		FMath::FloorToInt(Pivot.X / SnapSize),
		FMath::FloorToInt(Pivot.Y / SnapSize),
		FMath::RoundToInt(Pivot.Z / SnapSize)
	);

	UE_LOG(LogTemp, Warning, TEXT("Pivot %s"), *Pivot.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Coord %s"), *GridCoord.ToString());

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Params.bNoFail = true;
	Params.Owner = this;

	APlacement* SpawnedActor = GetWorld()->SpawnActor<APlacement>(PlacementClass, Location, Rotation, Params);
		
	if (!SpawnedActor)
	{
		return;
	}

	SpawnedActor->SetReplicates(true);
	SpawnedActor->SetReplicatingMovement(true);
	if (SpawnedActor->GetMeshAssetPath().IsValid())
	{
		SpawnedActor->SetMeshAssetPath(SpawnedActor->GetMeshAssetPath());
		SpawnedActor->OnRep_LoadMeshAsset();  // 서버 즉시 적용
	}

	SpawnedActor->SetCachedSnapSize(SnapSize);
	SpawnedActor->ForceNetUpdate();
	SpawnedActor->Setup(SnapSize);

	// UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("BuildPlacement %s"), 
	// *SpawnedActor
	// ->GetActorNameOrLabel()));
		
	PlacementGridContainer.Add(GridCoord, SpawnedActor, ItemId);
}

void AGridManager::BuildPlacementAtGhost(TSubclassOf<APlacement> PlacementClass,
	uint16 ItemId, const APlacement& Ghost)
{
	BuildPlacement(PlacementClass, ItemId, Ghost.GetActorPivotLocation(), Ghost.GetActorLocation(),
						  Ghost.GetActorRotation());
}

uint16 AGridManager::RemovePlacement(const FIntVector& GridAt)
{
	TWeakObjectPtr<APlacement> Placement = PlacementGridContainer.GetPlacedMap().FindRef(GridAt);
	if (Placement.IsValid())
	{
		const uint16 ItemId = PlacementGridContainer.Remove(Placement.Get());

		// 가구 제거
		Placement->Destroy();

		return ItemId;
	}

	return 0;
}

bool AGridManager::TryGetPlacement(APlacement* Placement, FIntVector& OutGridAt, APlacement*& OutPlacement)
{
	return TryGetPlacement(Placement->GetActorPivotLocation(), OutGridAt, OutPlacement);
}

bool AGridManager::TryGetPlacement(const FVector& Location, FIntVector& OutGridAt, APlacement*& OutPlacement)
{
	FIntVector ToCoord = WorldToGridLocation(Location);

	if (PlacementGridContainer.GetPlacedMap().Contains(ToCoord))
	{
		TWeakObjectPtr<APlacement> Placement = PlacementGridContainer.GetPlacedMap()[ToCoord];
		if (Placement.IsValid())
		{
			OutGridAt = ToCoord;
			OutPlacement = Placement.Get();

			return true;
		}
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

void AGridManager::SetVisibleGrid(bool bIsVisible)
{
	if (!GridComp)
	{
		return;
	}

	GridComp->SetVisibility(bIsVisible);
}
 
void AGridManager::Server_BuildPlacement_Implementation
(TSubclassOf<APlacement> PlacementClass, uint16 ItemId, FVector Pivot,
                                                        FVector Location, FRotator Rotation)
{
	BuildPlacement(PlacementClass, ItemId, Pivot, Location, Rotation);
}
