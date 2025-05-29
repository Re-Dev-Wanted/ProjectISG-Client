#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTags.h"
#include "ProjectISG/GAS/Common/Object/BaseInteractiveActor.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "Placement.generated.h"

// 그리드에 place할 모든 것들은 이것을 상속 받아야함

class UArrowComponent;

UCLASS()
class PROJECTISG_API APlacement : public ABaseInteractiveActor
{
	GENERATED_BODY()

public:
	APlacement();

	GETTER_SETTER(TSoftObjectPtr<UStaticMesh>, MeshAssetPath)
	SETTER(float, CachedSnapSize)

	virtual void OnSpawned()
	{
	}

	virtual bool GetCanTouch() const override;

	virtual FString GetTouchDisplayText(AActor* Causer) const override;

	virtual void OnTouch(AActor* Causer) override;

protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(
		TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FVector MeshSize = FVector::ZeroVector;

	UPROPERTY(Replicated)
	float CachedSnapSize;

	UPROPERTY(Replicated)
	bool bCanInteractive;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* AnchorComp;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UProceduralMeshComponent* ProceduralMeshComp;

	UPROPERTY(EditAnywhere, Replicated, ReplicatedUsing = OnRep_LoadMeshAsset)
	TSoftObjectPtr<UStaticMesh> MeshAssetPath;

	UPROPERTY(EditAnywhere)
	UMaterialInstance* TempMaterial;

	UPROPERTY(EditAnywhere)
	USceneComponent* InteractStartPoint;

	UPROPERTY(EditAnywhere)
	USceneComponent* InteractEndPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FIntVector> Occupied;

	virtual void Setup(float TileSize);

	void SetGuide(float TileSize);

	virtual void SetOption(bool bIsGhost, bool bIsBlock = false);

	void SetCollisionEnabled(bool bEnable) const;

	TArray<FIntVector> GetOccupiedGrid(float SnapSize,
	                                   const FIntVector& Current);

	FVector GetActorPivotLocation() const;

	UFUNCTION()
	void OnRep_LoadMeshAsset();

	FVector GetStartInteractPoint() const;

	FRotator GetStartInteractRotation() const;

	FVector GetEndInteractPoint() const;

	FRotator GetEndInteractRotation() const;

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetCollisionEnabled(bool bEnable) const;

	UFUNCTION(Server, Reliable)
	void Server_SetInteractingPlayer(AActor* Actor);

	UPROPERTY(EditDefaultsOnly, Category = "Options",
		meta = (AllowPrivateAccess = true))
	FGameplayTag OnPlaceCueTag;

	UPROPERTY(EditDefaultsOnly, Category = "Options",
		meta = (AllowPrivateAccess = true))
	FGameplayTag OnRemoveCueTag;
};
