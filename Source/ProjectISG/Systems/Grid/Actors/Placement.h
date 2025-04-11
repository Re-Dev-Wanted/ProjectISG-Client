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

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* AnchorComp;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere)
	class UStaticMesh* StaticMesh;

	UPROPERTY(EditAnywhere)
	class UMaterialInstance* TempMaterial;

	virtual void Setup(float TileSize);

	void SetColor(bool bIsGhost, bool bIsBlock);
};
