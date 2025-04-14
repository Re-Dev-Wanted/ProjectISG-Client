#include "Placement.h"
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

	FVector BoxExtent = StaticMesh->GetBounds().BoxExtent;

	float HalfSize = FMath::RoundToInt(TileSize * 0.5f);

	FVector SnapExtent
	(
		FMath::CeilToInt(BoxExtent.X / HalfSize) * HalfSize,
		FMath::CeilToInt(BoxExtent.Y / HalfSize) * HalfSize,
		FMath::CeilToInt(BoxExtent.Z / HalfSize) * HalfSize
	);

	CollisionComp->SetBoxExtent(SnapExtent);
	CollisionComp->SetRelativeLocation(SnapExtent);

	MeshComp->SetStaticMesh(StaticMesh);
	MeshComp->SetRelativeLocation(FVector::DownVector * SnapExtent.Z);
}

void APlacement::SetColor(bool bIsGhost, bool bIsBlock)
{
	if (bIsGhost && TempMaterial)
	{
		MeshComp->SetMaterial(0, TempMaterial);
		UMaterialInstanceDynamic* MatDynamic = MeshComp->CreateAndSetMaterialInstanceDynamic(0);
		MatDynamic->SetVectorParameterValue("HighlightColor", bIsBlock ? FLinearColor::Red : FLinearColor::Green);
	}
}
