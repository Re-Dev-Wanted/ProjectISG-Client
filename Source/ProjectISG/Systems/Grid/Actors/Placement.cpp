#include "Placement.h"

#include "ProceduralMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

APlacement::APlacement()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	AnchorComp = CreateDefaultSubobject<USceneComponent>(TEXT("AnchorComp"));
	AnchorComp->SetupAttachment(RootComponent);

	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	CollisionComp->SetupAttachment(AnchorComp);
	CollisionComp->SetIsReplicated(true);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(CollisionComp);

	ProceduralMeshComp = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMeshComp"));
	ProceduralMeshComp->SetupAttachment(CollisionComp);
	ProceduralMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProceduralMeshComp->SetGenerateOverlapEvents(false);

	bReplicates = true;
	bAlwaysRelevant = true;
	SetReplicatingMovement(true);
}

void APlacement::BeginPlay()
{
	Super::BeginPlay();
	
	// 서버, 클라 구분 없이 강제 Mesh 세팅
	UStaticMesh* Mesh = nullptr;
	if (MeshAssetPath.IsValid())
	{
		Mesh = MeshAssetPath.Get();
	}
	else
	{
		Mesh = MeshAssetPath.LoadSynchronous();
	}
	
	if (Mesh)
	{
		MeshComp->SetStaticMesh(Mesh);
		Setup(CachedSnapSize);
		// UE_LOG(LogTemp, Warning, TEXT("BeginPlay에서 Mesh 로딩 완료: %s"), *Mesh->GetName());
	}
	// else
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("BeginPlay: MeshAssetPath 로딩 실패: %s"), *MeshAssetPath.ToString());
	// }
}

void APlacement::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlacement, CachedSnapSize);
	DOREPLIFETIME(APlacement, MeshAssetPath);
}

void APlacement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlacement::Setup(float TileSize)
{
	UStaticMesh* Mesh = MeshAssetPath.IsValid() ? MeshAssetPath.Get() : MeshAssetPath.LoadSynchronous();
	if (!Mesh)
	{
		UE_LOG(LogTemp, Error, TEXT("MeshAssetPath not loaded in Setup"));
		return;
	}

	MeshComp->SetStaticMesh(Mesh);
	
	// 오브젝트 크기가 제각각 다를 것이다.
	// 적당히 가운데 정렬하고 tileSize에 맞추기

	FVector BoxExtent = Mesh->GetBounds().BoxExtent;
	
	float HalfSize = FMath::FloorToInt(TileSize * 0.5f);

	FVector SnapExtent
	(
		FMath::CeilToInt(BoxExtent.X / HalfSize) * HalfSize,
		FMath::CeilToInt(BoxExtent.Y / HalfSize) * HalfSize,
		FMath::CeilToInt(BoxExtent.Z / HalfSize) * HalfSize
	);

	FVector FixedLocation = FVector::UpVector * SnapExtent.Z;

	if (!CollisionComp->IsRegistered())
	{
		CollisionComp->RegisterComponent(); // 강제 등록
	}

	CollisionComp->SetBoxExtent(SnapExtent);

	FVector PivotLocation = FVector
	(
		FMath::Max(0, FMath::FloorToInt(SnapExtent.X / HalfSize) - 1) * HalfSize,
		FMath::Max(0, FMath::FloorToInt(SnapExtent.Y / HalfSize) - 1) * HalfSize,
		SnapExtent.Z
	);

	CollisionComp->SetRelativeLocation(PivotLocation);
	MeshComp->SetRelativeLocation(-FixedLocation);

	CollisionComp->RecreatePhysicsState();

	MeshSize = SnapExtent * 2.f;

	TArray<FVector> EmptyVectorArray;
	TArray<FVector2D> EmptyVector2DArray;
	TArray<FColor> EmptyColorArray;
	TArray<FProcMeshTangent> EmptyTangentArray;

	TArray<FVector> ProceduralVertices;
	TArray<int32> ProceduralTriangles;

	const int32 Rows = FMath::CeilToInt(BoxExtent.Y / HalfSize);
	const int32 Columns = FMath::CeilToInt(BoxExtent.X / HalfSize);

	for (int32 y = 0; y < Rows; y++)
	{
		for (int32 x = 0; x < Columns; x++)
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

	if (ProceduralMeshComp && ProceduralVertices.Num() > 0 && ProceduralTriangles.Num() > 0)
	{
		if (!ProceduralMeshComp->IsRegistered())
		{
			ProceduralMeshComp->RegisterComponent(); // 강제 등록
		}
		
		ProceduralMeshComp->CreateMeshSection(0, ProceduralVertices, ProceduralTriangles, EmptyVectorArray,
											  EmptyVector2DArray, EmptyColorArray,
											  EmptyTangentArray, false);

		ProceduralMeshComp->SetRelativeLocation(-SnapExtent);
		ProceduralMeshComp->SetVisibility(true);
		ProceduralMeshComp->SetCastShadow(false);
	}
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

TArray<FIntVector> APlacement::GetOccupiedGrid(float SnapSize, const FIntVector& Current)
{
	TArray<FIntVector> Array;
	Occupied.Empty();


	FVector BoxExtent = CollisionComp->Bounds.BoxExtent;

	float HalfSize = SnapSize * 0.5f;

	int32 Rows = FMath::FloorToInt(BoxExtent.Y / HalfSize);
	int32 Columns = FMath::FloorToInt(BoxExtent.X / HalfSize);

	int32 Yaw = FMath::FloorToInt(GetActorRotation().Yaw);

	// DrawDebugBox(GetWorld(), CollisionComp->Bounds.Origin, BoxExtent, FColor::Red, false, 1.f);

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

			Array.Add(Current + Offset);
		}
	}

	Occupied.Append(Array);

	return Array;
}

FVector APlacement::GetActorPivotLocation() const
{
	FVector PivotLocation = AnchorComp->GetComponentLocation();
	return FVector(PivotLocation.X, PivotLocation.Y, 0);
}

void APlacement::OnRep_LoadMeshAsset()
{
	if (MeshAssetPath.IsValid())
	{
		MeshComp->SetStaticMesh(MeshAssetPath.Get());
	}
	else
	{
		// 동기 로딩... 흠...
		UStaticMesh* LoadedMesh = MeshAssetPath.LoadSynchronous();
		if (LoadedMesh)
		{
			MeshComp->SetStaticMesh(LoadedMesh);
			Setup(CachedSnapSize);
		}
		else
		{
			UKismetSystemLibrary::PrintString(GetWorld(), TEXT("MeshAssetPath is invalid or not loaded"));
		}
	}
}
