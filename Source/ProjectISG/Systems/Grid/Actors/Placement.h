#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectISG/GAS/Common/Object/BaseActor.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "Placement.generated.h"

// 그리드에 place할 모든 것들은 이것을 상속 받아야함

UCLASS()
class PROJECTISG_API APlacement : public ABaseActor
{
	GENERATED_BODY()

public:
	APlacement();
	
	GETTER_SETTER(TSoftObjectPtr<UStaticMesh>, MeshAssetPath)
	GETTER(FVector, MeshSize)
	SETTER(float, CachedSnapSize)

protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	FVector MeshSize = FVector::ZeroVector;

	UPROPERTY(Replicated)
	float CachedSnapSize;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* Root;

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

	UPROPERTY(EditAnywhere, Replicated, ReplicatedUsing = OnRep_LoadMeshAsset)
	TSoftObjectPtr<UStaticMesh> MeshAssetPath;

	UPROPERTY(EditAnywhere)
	class UMaterialInstance* TempMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FIntVector> Occupied;

	virtual void Setup(float TileSize);

	void SetColor(bool bIsGhost, bool bIsBlock);

	TArray<FIntVector> GetOccupiedGrid(float SnapSize, const FIntVector& Current);

	FVector GetActorPivotLocation() const;

	UFUNCTION()
	void OnRep_LoadMeshAsset();
};
