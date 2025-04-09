// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grid.generated.h"

UCLASS()
class COMMON_API AGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	class UProceduralMeshComponent* SquareProceduralMeshComp;

	UPROPERTY(VisibleAnywhere)
	class UProceduralMeshComponent* LineProceduralMeshComp;

	UFUNCTION()
	void DrawLine(FVector Start, FVector End, float Thickness, TArray<FVector> Vertices, TArray<int32> Triangles);

	UPROPERTY(EditAnywhere)
	int32 Rows = 10;

	UPROPERTY(EditAnywhere)
	int32 Columns = 10;

	UPROPERTY(EditAnywhere) 
	float TileSize = 100.f;

	UPROPERTY(EditAnywhere)
	float LineThickness = 10.f;

	UPROPERTY(EditAnywhere)
	FLinearColor LineColor = FLinearColor::White;

	UPROPERTY(EditAnywhere)
	FLinearColor SquareColor = FLinearColor::White;

	UPROPERTY(EditAnywhere)
	float LineOpacity = 1;

	UPROPERTY(EditAnywhere)
	float SquareOpacity = 0.25f;

	UPROPERTY(EditAnywhere)
	TArray<FVector> GridVertices;

	UPROPERTY(EditAnywhere)
	TArray<int32> GridTriangles;

	UPROPERTY(EditAnywhere)
	TArray<int32> GridTriangles2;
	
private:
	float LineWidth() const
	{
		return Rows * TileSize;
	}

	float LineHeight() const
	{
		return Columns * TileSize;
	}

};
