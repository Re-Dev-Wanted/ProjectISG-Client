// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grid.generated.h"

UCLASS()
class PROJECTISG_API AGrid : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGrid();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UProceduralMeshComponent* LineMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UProceduralMeshComponent* SquareMeshComp;

	UPROPERTY(EditAnywhere)
	class UMaterialInstance* GridMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Rows = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Columns = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	float TileSize = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LineThickness = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor LineColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor SquareColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LineOpacity = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SquareOpacity = 0.25f;
	
private:
	void DrawLine(FVector Start, FVector End, float Thickness, TArray<FVector>& Vertices, TArray<int32>& Triangles);
	
	UMaterialInstanceDynamic* CreateMaterialInstance(FLinearColor Color, float Opacity, float Power = 2.f) const;
	
	float LineWidth() const
	{
		return Rows * TileSize;
	}

	float LineHeight() const
	{
		return Columns * TileSize;
	}
};
