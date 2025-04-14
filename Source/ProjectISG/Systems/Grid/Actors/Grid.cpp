// Fill out your copyright notice in the Description page of Project Settings.

#include "Grid.h"
#include "ProceduralMeshComponent.h"
#include "Kismet/KismetMaterialLibrary.h"

// Sets default values
AGrid::AGrid()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LineMeshComp = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Lines_ProceduralMesh"));
	SquareMeshComp = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Squares_ProceduralMesh"));

	ConstructorHelpers::FObjectFinder<UMaterialInstance> GridMat(TEXT("/Game/Systems/Grid/M_Grid_Inst.M_Grid_Inst"));

	if (GridMat.Succeeded())
	{
		GridMaterialInstance = GridMat.Object;
	}
}

void AGrid::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	UMaterialInstanceDynamic* LineMaterialInstance = CreateMaterialInstance(LineColor, LineOpacity);
	UMaterialInstanceDynamic* SquereMaterialInstance = CreateMaterialInstance(SquareColor, SquareOpacity);

	float LineStart;
	float LineEnd;
	TArray<FVector> LineVertices;
	TArray<int32> LineTriangles;
	TArray<FVector> SquareVertices;
	TArray<int32> SquareTriangles;

	// Horizontal Lines
	for (int32 Row = 0; Row <= Rows; Row++)
	{
		LineStart = TileSize * Row;
		LineEnd = LineWidth();

		DrawLine(FVector(LineStart, 0, 0), FVector(LineStart, LineEnd, 0), LineThickness, LineVertices, LineTriangles);
	}

	// Vertical Lines
	for (int32 Column = 0; Column <= Columns; Column++)
	{
		LineStart = TileSize * Column;
		LineEnd = LineHeight();

		DrawLine(FVector(0, LineStart, 0), FVector(LineEnd, LineStart, 0), LineThickness, LineVertices, LineTriangles);
	}
	
	TArray<FVector> EmptyVectorArray;
	TArray<FVector2D> EmptyVector2DArray;
	TArray<FColor> EmptyColorArray;
	TArray<FProcMeshTangent> EmptyTangentArray;

	LineMeshComp->CreateMeshSection(0, LineVertices, LineTriangles, EmptyVectorArray, EmptyVector2DArray, EmptyColorArray, EmptyTangentArray, false);
	LineMeshComp->SetMaterial(0, LineMaterialInstance);
}

// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGrid::DrawLine(FVector Start, FVector End, float Thickness, TArray<FVector>& Vertices, TArray<int32>& Triangles)
{
	float HalfThickness = Thickness * 0.5f;
	FVector Direction = (End - Start).GetSafeNormal().Cross(FVector::UpVector);

	// [0 1]
	// [2 3]
	// 2->1->0, 2->3->1

	int32 Num = Vertices.Num();

	TArray<int32> Pins
	{
		Num + 2,
		Num + 1,
		Num,
		Num + 2,
		Num + 3,
		Num + 1
	};

	Triangles.Append(Pins);

	FVector Offset = Direction * HalfThickness;

	TArray<FVector> Corners
	{
		Start + Offset, // Top-Left
		End + Offset, // Top-Right
		Start - Offset, // Bottom-Left
		End - Offset // Bottom-Right
	};

	Vertices.Append(Corners);
}

UMaterialInstanceDynamic* AGrid::CreateMaterialInstance(FLinearColor Color, float Opacity, float Power) const
{
	UMaterialInstanceDynamic* MaterialInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), GridMaterialInstance);
	MaterialInstance->SetVectorParameterValue("Color", Color);
	MaterialInstance->SetScalarParameterValue("Opacity", Opacity);
	MaterialInstance->SetScalarParameterValue("Power", Power);

	return MaterialInstance;
}
