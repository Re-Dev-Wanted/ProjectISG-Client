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

	UPROPERTY()
	TSubclassOf<APlacement> TestPlacementClass;

public:
	TMap<FIntVector, APlacement*> PlacedMap; // 배치되어 있는 것들 정보

	// UPROPERTY(EditAnywhere, Category=BaseProperties)
	// int32 Rows = 10;
	//
	// UPROPERTY(EditAnywhere, Category=BaseProperties)
	// int32 Column = 10;

	UPROPERTY(EditAnywhere, Category=BaseProperties)
	float SnapSize = 100;

	FVector SnapToGrid(const FVector& Location);

	FIntVector WorldToGridLocation(const FVector& WorldLocation);

	FVector GridToWorldLocation(const FIntVector& GridCoord);

	FVector GetLocationInFront(AActor* Actor, int32 Distance = 1);

	template <class T, std::enable_if_t<std::is_base_of_v<APlacement, T>, int>  = 0>
	void BuildPlacement(TSubclassOf<T> PlacementClass, const FVector& Location)
	{
		FActorSpawnParameters Params;

		T* SpawnedActor = GetWorld()->SpawnActor<T>(PlacementClass, SnapToGrid(Location), FRotator::ZeroRotator,
		                                            Params);
		SpawnedActor->Setup();

		// Map에 저장
		PlacedMap.Add(WorldToGridLocation(Location), SpawnedActor);
	}

	template <class T, std::enable_if_t<std::is_base_of_v<APlacement, T>, int>  = 0>
	void BuildPlacementInFrontOfActor(TSubclassOf<T> PlacementClass, AActor* Actor)
	{
		FVector BuildLocation = GetLocationInFront(Actor, 1);
		BuildPlacement<T>(PlacementClass.BuildLocation);
	}

	void RemovePlacement(const FIntVector& GridAt);

	bool TryGetPlacementAt(AActor* Actor, FIntVector& OutGridAt, APlacement*& OutPlacement);

	// float GetGridWidth() const
	// {
	// 	return Rows * SnapSize;
	// }
	//
	// float GetGridHeight() const
	// {
	// 	return Column * SnapSize;
	// }
};
