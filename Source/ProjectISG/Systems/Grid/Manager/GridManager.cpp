#include "GridManager.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "Net/UnrealNetwork.h"
#include "ProjectISG/GAS/Common/Tag/ISGGameplayTag.h"
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

	if (!HasAuthority())
	{
		Destroy();
	}

	SetVisibleGrid(false);
}

void AGridManager::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (GridComp)
	{
		GridComp->OnUpdatedProperties();
	}
}

void AGridManager::GetLifetimeReplicatedProps(
	TArray<class FLifetimeProperty>& OutLifetimeProps) const
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
		0.f
		// FMath::FloorToInt(Location.Z / SnapSize) * SnapSize
	);
}

FVector AGridManager::SnapToGridPlacement(const FVector& Location)
{
	FIntVector Grid = WorldToGridLocation(Location);
	return GridToWorldLocation(Grid);
}

FIntVector AGridManager::WorldToGridLocation(const FVector& WorldLocation)
{
	const FVector LocalLocation = GetActorTransform().InverseTransformPosition(
		WorldLocation);

	return FIntVector
	(
		FMath::FloorToInt(LocalLocation.X / SnapSize),
		FMath::FloorToInt(LocalLocation.Y / SnapSize),
		0.f
		// FMath::FloorToInt(WorldLocation.Z / SnapSize)
	);
}

FVector AGridManager::GridToWorldLocation(const FIntVector& GridCoord)
{
	const FVector LocalLocation = FVector
	(
		FMath::FloorToInt(GridCoord.X * SnapSize + SnapSize * 0.5f),
		FMath::FloorToInt(GridCoord.Y * SnapSize + SnapSize * 0.5f),
		GridCoord.Z * SnapSize // 보통 Z는 그대로
	);

	return GetActorTransform().TransformPosition(LocalLocation);
}

FVector AGridManager::GetLocationInFront(AActor* Actor, int32 Distance)
{
	FVector Forward = Actor->GetActorForwardVector();
	FVector Origin = Actor->GetActorLocation();

	FVector RawTarget = Origin + Forward * Distance * SnapSize;
	return SnapToGridPlacement(RawTarget);
}

FVector AGridManager::GetLocationInPointerDirection(
	APlayerController* PlayerController, int32 Distance)
{
	if (!PlayerController)
	{
		return FVector::ZeroVector;
	}

	FHitResult HitResult;
	if (PlayerController->GetHitResultUnderCursor(
			ECC_Visibility, false, HitResult) && HitResult.
		bBlockingHit)
	{
		return SnapToGrid(HitResult.ImpactPoint);
	}

	return FVector::ZeroVector;
}

FVector AGridManager::GetLocationInPointerDirectionPlacement(
	APlayerController* PlayerController, FVector MeshSize,
	int32 Distance)
{
	if (!PlayerController)
	{
		return FVector::ZeroVector;
	}

	FHitResult HitResult;
	if (PlayerController->GetHitResultUnderCursor(
			ECC_Visibility, false, HitResult) && HitResult.
		bBlockingHit)
	{
		return SnapToGridPlacement(HitResult.ImpactPoint);
	}

	return FVector::ZeroVector;
}

void AGridManager::BuildPlacement(TSubclassOf<APlacement> PlacementClass,
                                  uint16 ItemId, const FVector& Pivot,
                                  const FVector& Location,
                                  const FRotator& Rotation)
{
	FIntVector GridCoord = WorldToGridLocation(Pivot);

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Params.bNoFail = true;
	Params.Owner = GetWorld()->GetFirstPlayerController();

	APlacement* SpawnedActor = GetWorld()->SpawnActor<APlacement>(
		PlacementClass, SnapToGridPlacement(Location), Rotation, Params);

	if (!SpawnedActor)
	{
		return;
	}

	SpawnedActor->SetReplicates(true);
	SpawnedActor->SetReplicatingMovement(true);
	SpawnedActor->bNetLoadOnClient = true;
	SpawnedActor->bAlwaysRelevant = true;
	if (SpawnedActor->GetMeshAssetPath().IsValid())
	{
		SpawnedActor->SetMeshAssetPath(SpawnedActor->GetMeshAssetPath());
		SpawnedActor->OnRep_LoadMeshAsset(); // 서버 즉시 적용
	}

	SpawnedActor->SetCachedSnapSize(SnapSize);
	SpawnedActor->ForceNetUpdate();
	SpawnedActor->Setup(SnapSize);
	SpawnedActor->SetOption(false);
	SpawnedActor->OnSpawned();

	PlacementGridContainer.Add
	(
		GridCoord,
		SpawnedActor,
		ItemId,
		[this, GridCoord, ItemId]
		{
			ForceNetUpdate();

			Multicast_BuildPlacement(GridCoord, ItemId);
		}
	);

	FGameplayCueParameters Param;
	Param.AbilityLevel = 1;
	Param.EffectCauser = SpawnedActor;
	SpawnedActor->GetAbilitySystemComponent()->ExecuteGameplayCue(
		SpawnedActor->OnPlaceCueTag, Param);
}

bool AGridManager::RemovePlacement(const FIntVector& GridAt, uint16 ItemId)
{
	ItemId = PlacedMap.FindRef(GridAt);

	if (ItemId > 0)
	{
		PlacementGridContainer.Remove
		(
			GridAt, ItemId,
			[this]
			{
				ForceNetUpdate();
			}
		);

		Multicast_RemovePlacement(GridAt);

		return true;
	}

	return false;
}

void AGridManager::Server_RemovePlacement_Implementation(
	const FIntVector& GridAt, uint16 ItemId)
{
	RemovePlacement(GridAt, ItemId);
}

void AGridManager::Multicast_RemovePlacement_Implementation(
	const FIntVector& GridAt)
{
	PlacedMap.Remove(GridAt);
}

bool AGridManager::TryGetPlacement(APlacement* Placement, FIntVector& OutGridAt,
                                   uint16& ItemId)
{
	return TryGetPlacement(Placement->GetActorPivotLocation(), OutGridAt,
	                       ItemId);
}

bool AGridManager::TryGetPlacement(const FVector& Location,
                                   FIntVector& OutGridAt, uint16& ItemId)
{
	FIntVector ToCoord = WorldToGridLocation(Location);

	ItemId = PlacedMap.FindRef(ToCoord);

	if (ItemId > 0)
	{
		OutGridAt = ToCoord;
		return true;
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

bool AGridManager::IsEmptyGrid(const FVector& Location)
{
	FIntVector ToCoord = WorldToGridLocation(Location);

	if (ToCoord.X < 0 || ToCoord.X >= Rows || ToCoord.Y < 0 || ToCoord.Y >=
		Columns)
	{
		return false;
	}

	uint16 ItemId = PlacedMap.FindRef(ToCoord);

	return ItemId <= 0;
}

void AGridManager::Server_BuildPlacement_Implementation
(TSubclassOf<APlacement> PlacementClass, uint16 ItemId, FVector Pivot,
 FVector Location, FRotator Rotation)
{
	BuildPlacement(PlacementClass, ItemId, Pivot, Location, Rotation);
}

void AGridManager::Multicast_BuildPlacement_Implementation(
	const FIntVector& GridAt, uint16 ItemId)
{
	PlacedMap.Add(GridAt, ItemId);
}
