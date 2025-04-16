// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Placement.generated.h"

// 그리드에 place할 모든 것들은 이것을 상속 받아야함

UCLASS()
class PROJECTISG_API APlacement : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APlacement();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	FVector MeshSize = FVector::ZeroVector;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* AnchorComp;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UProceduralMeshComponent* ProceduralMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int32, UStaticMesh*> MeshMap;

	UPROPERTY(EditAnywhere)
	class UStaticMesh* BaseStaticMesh;

	UPROPERTY(EditAnywhere)
	class UMaterialInstance* TempMaterial;

	virtual void Setup(float TileSize);

	void SetColor(bool bIsGhost, bool bIsBlock);

	TArray<FIntVector> GetOccupiedGrid(float SnapSize, const FIntVector& Start);

	FVector GetMeshSize() const
	{
		return MeshSize;
	}
};
