#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectISG/Systems/Grid/PlacementGridContainer.h"
#include "GridManager.generated.h"

UCLASS()
class PROJECTISG_API AGridManager : public AActor
{
	GENERATED_BODY()

public:
	AGridManager();

protected:
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere)
	class UGridComponent* GridComp;

public:
	UPROPERTY(EditAnywhere, Category = Properties)
	int32 Rows = 10;

	UPROPERTY(EditAnywhere, Category = Properties)
	int32 Columns = 10;

	UPROPERTY(EditAnywhere, Category = Properties)
	float SnapSize = 100;

	UPROPERTY(EditAnywhere, Category = Properties)
	float Tolerance = 1.f;

	UPROPERTY(Replicated)
	FPlacementGridContainer PlacementGridContainer;

	FVector SnapToGrid(const FVector& Location);

	FVector SnapToGridPlacement(const FVector& Location);

	FIntVector WorldToGridLocation(const FVector& WorldLocation);

	FVector GridToWorldLocation(const FIntVector& GridCoord);

	FVector GetLocationInFront(AActor* Actor, int32 Distance = 1);

	FVector GetLocationInPointerDirection(APlayerController* PlayerController, int32 Distance = 1);

	FVector GetLocationInPointerDirectionPlacement(APlayerController* PlayerController, FVector MeshSize,
	                                               int32 Distance = 1);

	template <class T, std::enable_if_t<std::is_base_of_v<APlacement, T>, int> = 0>
	void BuildPlacement(TSubclassOf<T> PlacementClass, FItemMetaInfo ItemMetaInfo, const FVector& Pivot, const FVector& Location,
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

		T* SpawnedActor = GetWorld()->SpawnActor<T>(PlacementClass, Location, Rotation,
		                                            Params);
		
		if (!SpawnedActor)
		{
			return;
		}

		SpawnedActor->SetReplicates(true);
		SpawnedActor->SetReplicatingMovement(true);

		if (T* ClassCDO = PlacementClass->template GetDefaultObject<T>())
		{
			if (ClassCDO->GetMeshAssetPath().IsValid())
			{
				SpawnedActor->SetMeshAssetPath(ClassCDO->GetMeshAssetPath());
				SpawnedActor->OnRep_LoadMeshAsset();  // 서버 즉시 적용
			}
		}

		SpawnedActor->SetCachedSnapSize(SnapSize);
		SpawnedActor->ForceNetUpdate();
		SpawnedActor->Setup(SnapSize);
		
		PlacementGridContainer.Add(GridCoord, SpawnedActor, ItemMetaInfo);
	}

	template <class T, std::enable_if_t<std::is_base_of_v<APlacement, T>, int> = 0>
	void BuildPlacementAtGhost(TSubclassOf<T> PlacementClass, FItemMetaInfo ItemMetaInfo, T* Ghost)
	{
		BuildPlacement<T>(PlacementClass, ItemMetaInfo, Ghost->GetActorPivotLocation(), Ghost->GetActorLocation(),
		                  Ghost->GetActorRotation());
	}

	template <class T, std::enable_if_t<std::is_base_of_v<APlacement, T>, int> = 0>
	void BuildPlacementInFrontOfActor(TSubclassOf<T> PlacementClass, FItemMetaInfo ItemMetaInfo, AActor* Actor, const FRotator& Rotation)
	{
		FVector BuildLocation = GetLocationInFront(Actor, 1);
		BuildPlacement<T>(PlacementClass, ItemMetaInfo, BuildLocation, Rotation);
	}

	UFUNCTION(Server, Reliable)
	void Server_BuildPlacement(TSubclassOf<APlacement> PlacementClass, FItemMetaInfo_Net ItemMetaInfo, FVector Pivot, FVector Location, FRotator Rotation);

	FItemMetaInfo RemovePlacement(const FIntVector& GridAt);

	bool TryGetPlacement(APlacement* Placement, FIntVector& OutGridAt, APlacement*& OutPlacement);

	bool TryGetPlacement(const FVector& Location, FIntVector& OutGridAt, APlacement*& OutPlacement);

	bool TryGetPlacementAt(AActor* Actor, FIntVector& OutGridAt, APlacement*& OutPlacement);

	float GetGridWidth() const
	{
		return Rows * SnapSize;
	}

	float GetGridHeight() const
	{
		return Columns * SnapSize;
	}
};
