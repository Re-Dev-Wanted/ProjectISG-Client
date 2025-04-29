// Fill out your copyright notice in the Description page of Project Settings.


#include "GridComponent.h"

#include "ProceduralMeshComponent.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "ProjectISG/Systems/Grid/Manager/GridManager.h"

UGridComponent::UGridComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	LineMeshComp = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Lines_ProceduralMesh"));
	SquareMeshComp = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Squares_ProceduralMesh"));

	ConstructorHelpers::FObjectFinder<UMaterialInstance> GridMat(TEXT("/Game/Systems/Grid/Materials/M_Grid_Inst.M_Grid_Inst"));

	if (GridMat.Succeeded())
	{
		GridMaterialInstance = GridMat.Object;
	}
}


void UGridComponent::OnRegister()
{
	Super::OnRegister();

	Manager = Cast<AGridManager>(GetOwner());

	OnUpdatedProperties();
}

void UGridComponent::OnUpdatedProperties()
{
	if (!Manager)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UGridComponent::OnRegister] AGridManager is Null"));
		return;
	}

	UMaterialInstanceDynamic* LineMaterialInstance = CreateMaterialInstance(LineColor, LineOpacity);
	UMaterialInstanceDynamic* SquereMaterialInstance = CreateMaterialInstance(SquareColor, SquareOpacity);

	float LineStart;
	float LineEnd;
	TArray<FVector> LineVertices;
	TArray<int32> LineTriangles;
	TArray<FVector> SquareVertices;
	TArray<int32> SquareTriangles;

	int32 Rows = Manager->Rows;
	int32 Columns = Manager->Columns;
	float TileSize = Manager->SnapSize;

	// Horizontal Lines
	for (int32 Row = 0; Row <= Rows; Row++)
	{
		LineStart = TileSize * Row;
		LineEnd = Manager->GetGridWidth();

		DrawLine(FVector(LineStart, 0, 0), FVector(LineStart, LineEnd, 0), LineThickness, LineVertices, LineTriangles);
	}

	// Vertical Lines
	for (int32 Column = 0; Column <= Columns; Column++)
	{
		LineStart = TileSize * Column;
		LineEnd = Manager->GetGridHeight();

		DrawLine(FVector(0, LineStart, 0), FVector(LineEnd, LineStart, 0), LineThickness, LineVertices, LineTriangles);
	}

	TArray<FVector> EmptyVectorArray;
	TArray<FVector2D> EmptyVector2DArray;
	TArray<FColor> EmptyColorArray;
	TArray<FProcMeshTangent> EmptyTangentArray;

	LineMeshComp->CreateMeshSection(0, LineVertices, LineTriangles, EmptyVectorArray, EmptyVector2DArray,
	                                EmptyColorArray, EmptyTangentArray, false);
	LineMeshComp->SetMaterial(0, LineMaterialInstance);
}

void UGridComponent::DrawLine(FVector Start, FVector End, float Thickness, TArray<FVector>& Vertices,
                              TArray<int32>& Triangles)
{
	float HalfThickness = Thickness * 0.5f;
	FVector Direction = (End - Start).GetSafeNormal().Cross(FVector::UpVector);

	// [0 1]
	// [2 3]
	// 2->1->0, 2->3->1

	int32 Num = Vertices.Num();

	TArray Pins
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

	TArray Corners
	{
		Start + Offset, // Top-Left
		End + Offset, // Top-Right
		Start - Offset, // Bottom-Left
		End - Offset // Bottom-Right
	};

	Vertices.Append(Corners);
}

UMaterialInstanceDynamic* UGridComponent::CreateMaterialInstance(FLinearColor Color, float Opacity, float Power) const
{
	UMaterialInstanceDynamic* MaterialInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(
		GetWorld(), GridMaterialInstance);
	MaterialInstance->SetVectorParameterValue("Color", Color);
	MaterialInstance->SetScalarParameterValue("Opacity", Opacity);
	MaterialInstance->SetScalarParameterValue("Power", Power);

	return MaterialInstance;
}
