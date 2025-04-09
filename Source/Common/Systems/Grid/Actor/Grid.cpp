// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"

#include "ProceduralMeshComponent.h"

// Sets default values
AGrid::AGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SquareProceduralMeshComp = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("SquareProceduralMeshComp"));

	SquareProceduralMeshComp->SetupAttachment(RootComponent);
	
	LineProceduralMeshComp = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("LineProceduralMeshComp"));

	LineProceduralMeshComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
	Super::BeginPlay();


	TArray<FVector> EmptyVectorArray;
	TArray<FVector2D> EmptyVector2DArray;
	TArray<FColor> EmptyColorArray;
	TArray<FProcMeshTangent> EmptyTangentArray;
	
	LineProceduralMeshComp->CreateMeshSection(0, GridVertices, GridTriangles, EmptyVectorArray, EmptyVector2DArray, EmptyColorArray, EmptyTangentArray, false);
	
}

// Called every frame
void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGrid::DrawLine(FVector Start, FVector End, float Thickness, TArray<FVector> Vertices, TArray<int32> Triangles)
{
	float HalfThickness = Thickness * 0.5f;
	FVector Direction = (End - Start).GetSafeNormal().Cross(FVector::UpVector);
}

