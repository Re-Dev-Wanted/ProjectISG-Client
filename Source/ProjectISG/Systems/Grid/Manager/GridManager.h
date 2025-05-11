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

	//빠른 접근용 Cache Map
	UPROPERTY()
	TMap<FIntVector, uint16> PlacedMap;

	FVector SnapToGrid(const FVector& Location);

	FVector SnapToGridPlacement(const FVector& Location);

	FIntVector WorldToGridLocation(const FVector& WorldLocation);

	FVector GridToWorldLocation(const FIntVector& GridCoord);

	FVector GetLocationInFront(AActor* Actor, int32 Distance = 1);

	FVector GetLocationInPointerDirection(APlayerController* PlayerController, int32 Distance = 1);

	FVector GetLocationInPointerDirectionPlacement(APlayerController* PlayerController, FVector MeshSize,
	                                               int32 Distance = 1);

	void BuildPlacement(TSubclassOf<APlacement> PlacementClass, uint16 ItemId, const FVector& Pivot, const FVector& Location,
	                    const FRotator& Rotation);

	UFUNCTION(Server, Reliable)
	void Server_BuildPlacement(TSubclassOf<APlacement> PlacementClass, uint16 ItemId, FVector Pivot, FVector Location, FRotator Rotation);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_BuildPlacement(const FIntVector& GridAt, uint16 ItemId);

	bool RemovePlacement(const FIntVector& GridAt, uint16 ItemId);

	UFUNCTION(Server, Reliable)
	void Server_RemovePlacement(const FIntVector& GridAt, uint16 ItemId);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_RemovePlacement(const FIntVector& GridAt);

	bool TryGetPlacement(APlacement* Placement, FIntVector& OutGridAt, uint16& ItemId);

	bool TryGetPlacement(const FVector& Location, FIntVector& OutGridAt, uint16& ItemId);

	void SetVisibleGrid(bool bIsVisible);

	float GetGridWidth() const
	{
		return Rows * SnapSize;
	}

	float GetGridHeight() const
	{
		return Columns * SnapSize;
	}

	FRotator GetSnappedRotation(float Degrees) const
	{
		const float Yaw = GetActorRotation().Yaw;

		return FRotator(0.f, Yaw + Degrees, 0.f);
	}
};
