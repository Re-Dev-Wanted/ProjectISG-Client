#include "Placement.h"

#include "ProceduralMeshComponent.h"
#include "Components/BoxComponent.h"


APlacement::APlacement()
{
	PrimaryActorTick.bCanEverTick = true;

	AnchorComp = CreateDefaultSubobject<USceneComponent>(TEXT("AnchorComp"));
	AnchorComp->SetupAttachment(RootComponent);

	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	CollisionComp->SetupAttachment(AnchorComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(CollisionComp);

	ProceduralMeshComp = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMeshComp"));
	ProceduralMeshComp->SetupAttachment(CollisionComp);
	ProceduralMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProceduralMeshComp->SetGenerateOverlapEvents(false);
}

void APlacement::BeginPlay()
{
	Super::BeginPlay();

	// Setup();
}

void APlacement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlacement::Setup(float TileSize)
{
	// 오브젝트 크기가 제각각 다를 것이다.
	// 적당히 가운데 정렬하고 tileSize에 맞추기

	FVector BoxExtent = BaseStaticMesh->GetBounds().BoxExtent;

	float HalfSize = FMath::RoundToInt(TileSize * 0.5f);

	FVector SnapExtent
	(
		FMath::CeilToInt(BoxExtent.X / HalfSize) * HalfSize,
		FMath::CeilToInt(BoxExtent.Y / HalfSize) * HalfSize,
		FMath::CeilToInt(BoxExtent.Z / HalfSize) * HalfSize
	);

	FVector FixedLocation = FVector::UpVector * SnapExtent.Z;

	CollisionComp->SetBoxExtent(SnapExtent);
	CollisionComp->SetRelativeLocation(SnapExtent);

	MeshComp->SetStaticMesh(BaseStaticMesh);
	MeshComp->SetRelativeLocation(-FixedLocation);

	MeshSize = SnapExtent * 2.f;

	TArray<FVector> EmptyVectorArray;
	TArray<FVector2D> EmptyVector2DArray;
	TArray<FColor> EmptyColorArray;
	TArray<FProcMeshTangent> EmptyTangentArray;

	TArray<FVector> ProceduralVertices;
	TArray<int32> ProceduralTriangles;

	for (int32 y = 0; y < FMath::CeilToInt(BoxExtent.Y / HalfSize); y++)
	{
		for (int32 x = 0; x < FMath::CeilToInt(BoxExtent.X / HalfSize); x++)
		{
			int32 Num = ProceduralVertices.Num();

			TArray Pins
			{
				Num + 2,
				Num + 1,
				Num,
				Num + 2,
				Num + 3,
				Num + 1
			};

			ProceduralTriangles.Append(Pins);

			TArray Vertices
			{
				FVector(x * TileSize, y * TileSize, 0),
				FVector((x + 1) * TileSize, y * TileSize, 0),
				FVector(x * TileSize, (y + 1) * TileSize, 0),
				FVector((x + 1) * TileSize, (y + 1) * TileSize, 0)
			};

			ProceduralVertices.Append(Vertices);
		}
	}

	ProceduralMeshComp->CreateMeshSection(0, ProceduralVertices, ProceduralTriangles, EmptyVectorArray,
	                                      EmptyVector2DArray, EmptyColorArray,
	                                      EmptyTangentArray, false);

	ProceduralMeshComp->SetRelativeLocation(-SnapExtent);
}

void APlacement::SetColor(bool bIsGhost, bool bIsBlock)
{
	if (bIsGhost && TempMaterial)
	{
		CollisionComp->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
		CollisionComp->SetGenerateOverlapEvents(false);
		MeshComp->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
		MeshComp->SetGenerateOverlapEvents(false);

		MeshComp->SetMaterial(0, TempMaterial);
		UMaterialInstanceDynamic* MatDynamic = MeshComp->CreateAndSetMaterialInstanceDynamic(0);
		MatDynamic->SetVectorParameterValue("HighlightColor", bIsBlock ? FLinearColor::Red : FLinearColor::Green);
	}
}

TArray<FIntVector> APlacement::GetOccupiedGrid(float SnapSize, const FIntVector& Start)
{
	TArray<FIntVector> Array;

	// DrawDebugBox(GetWorld(), FVector(Start) * SnapSize, FVector(5, 5, 50), FColor::Red, false, 5.f);

	FVector BoxExtent = CollisionComp->Bounds.BoxExtent;

	float HalfSize = FMath::RoundToInt(SnapSize * 0.5f);

	int32 Rows = FMath::RoundToInt(BoxExtent.Y / HalfSize);
	int32 Columns = FMath::RoundToInt(BoxExtent.X / HalfSize);

	int32 Yaw = FMath::FloorToInt(GetActorRotation().Yaw);

	// Normalize yaw to 0, 90, 180, 270
	int32 NormalizedYaw = ((Yaw % 360) + 360) % 360;

	for (int32 x = 0; x < Columns; ++x)
	{
		for (int32 y = 0; y < Rows; ++y)
		{
			FIntVector Offset;

			switch (NormalizedYaw)
			{
			case 90: // 동: 좌상단 기준
				Offset = FIntVector(-y, x, 0);
				break;

			case 180: // 남: 우상단 기준
				Offset = FIntVector(-x, -y, 0);
				break;

			case 270: // 서: 우하단 기준
				Offset = FIntVector(y, -x, 0);
				break;

			default: // 북: 좌하단 기준
				Offset = FIntVector(x, y, 0);
				break;
			}

			Array.Add(Start + Offset);
		}
	}


	for (int32 y = 0; y < Rows; y++)
	{
		for (int32 x = 0; x < Columns; x++)
		{
			Array.Add(FIntVector(Start.X + x, Start.Y + y, Start.Z));
		}
	}

	return Array;
}
