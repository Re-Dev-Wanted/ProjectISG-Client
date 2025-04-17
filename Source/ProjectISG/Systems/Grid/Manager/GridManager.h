#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectISG/Systems/Grid/Actors/Placement.h"
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

	UPROPERTY(VisibleAnywhere)
	class UGridComponent* GridComp;

public:
	UPROPERTY(VisibleAnywhere)
	TMap<FIntVector, APlacement*> PlacedMap; // 배치되어 있는 것들 정보

	TMap<APlacement*, TArray<FIntVector>> ReverseMap; // 삭제를 편하게 하기 위해

	UPROPERTY(EditAnywhere, Category = Properties)
	int32 Rows = 10;

	UPROPERTY(EditAnywhere, Category = Properties)
	int32 Columns = 10;

	UPROPERTY(EditAnywhere, Category = Properties)
	float SnapSize = 100;

	FVector SnapToGrid(const FVector& Location);

	FVector SnapToGridPlacement(const FVector& Location);

	FIntVector WorldToGridLocation(const FVector& WorldLocation);

	FVector GridToWorldLocation(const FIntVector& GridCoord);

	FVector GetLocationInFront(AActor* Actor, int32 Distance = 1);

	FVector GetLocationInPointerDirection(APlayerController* PlayerController, int32 Distance = 1);

	FVector GetLocationInPointerDirectionPlacement(APlayerController* PlayerController, FVector MeshSize,
	                                               int32 Distance = 1);

	template <class T, std::enable_if_t<std::is_base_of_v<APlacement, T>, int>  = 0>
	void BuildPlacement(TSubclassOf<T> PlacementClass, const FVector& Pivot, const FVector& Location,
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

		if (PlacedMap.FindRef(GridCoord))
		{
			return;
		}

		FActorSpawnParameters Params;

		T* SpawnedActor = GetWorld()->SpawnActor<T>(PlacementClass, Location, Rotation,
		                                            Params);
		SpawnedActor->Setup(SnapSize);

		TArray<FIntVector> OccupiedCells = SpawnedActor->GetOccupiedGrid(SnapSize, GridCoord);

		ReverseMap.Add(SpawnedActor, TArray<FIntVector>());

		for (const FIntVector& Coord : OccupiedCells)
		{
			// Map에 저장
			PlacedMap.Add(Coord, SpawnedActor);
			ReverseMap[SpawnedActor].Add(Coord);
		}
	}

	template <class T, std::enable_if_t<std::is_base_of_v<APlacement, T>, int>  = 0>
	void BuildPlacementAtGhost(TSubclassOf<T> PlacementClass, T* Ghost)
	{
		BuildPlacement<T>(PlacementClass, Ghost->GetActorPivotLocation(), Ghost->GetActorLocation(),
		                  Ghost->GetActorRotation());
	}

	template <class T, std::enable_if_t<std::is_base_of_v<APlacement, T>, int>  = 0>
	void BuildPlacementInFrontOfActor(TSubclassOf<T> PlacementClass, AActor* Actor, const FRotator& Rotation)
	{
		FVector BuildLocation = GetLocationInFront(Actor, 1);
		BuildPlacement<T>(PlacementClass, BuildLocation, Rotation);
	}

	void RemovePlacement(const FIntVector& GridAt);

	bool TryGetPlacement(APlacement* Placement, FIntVector& OutGridAt, APlacement*& OutPlacement);

	bool TryGetPlacement(const FVector& Location, FIntVector& OutGridAt, APlacement*& OutPlacement);

	bool TryGetPlacementAt(AActor* Actor, FIntVector& OutGridAt, APlacement*& OutPlacement);

	// bool IsOverlapped(APlacement* Ghost);

	float GetGridWidth() const
	{
		return Rows * SnapSize;
	}

	float GetGridHeight() const
	{
		return Columns * SnapSize;
	}
};
