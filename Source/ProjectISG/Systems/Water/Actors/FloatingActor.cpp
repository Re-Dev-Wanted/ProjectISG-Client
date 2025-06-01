#include "FloatingActor.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ProjectISG/Systems/Water/Components/BuoyancyComponent.h"

AFloatingActor::AFloatingActor()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Root->SetSimulatePhysics(true);
	Root->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(Root);
	MeshComponent->bRenderCustomDepth = true;
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BuoyancyComponent = CreateDefaultSubobject<UBuoyancyComponent>(TEXT("BuoyancyComponent"));
}

bool AFloatingActor::CanApplyBuoyancy() const
{
	return true;
}

float AFloatingActor::GetBuoyancyScale() const
{
	return 1.f;
}

float AFloatingActor::GetActorBottomZ() const
{
	FVector Origin;
	FVector Extent;
	
	GetActorBounds(true, Origin, Extent);
	
	return Origin.Z - Extent.Z;
}

void AFloatingActor::SetCollisionAndPhysicsEnabled(bool bIsEnabled)
{
	Root->SetSimulatePhysics(bIsEnabled);

	ECollisionEnabled::Type CollisionEnabledType = bIsEnabled? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision;
	Root->SetCollisionEnabled(CollisionEnabledType);
}

void AFloatingActor::EnterWater()
{
	// UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("시작 %d"), BuoyancyComponent->GetIsInWater()));
	
	if (BuoyancyComponent->GetIsInWater())
	{
		return;
	}
	
	BuoyancyComponent->SetIsInWater(true);

	if (OnEnterWater.IsBound())
	{
		OnEnterWater.Execute();
	}
}

void AFloatingActor::ExitWater()
{
	GetBuoyancyComponent()->SetIsInWater(false);
	SetDamping(0.01f, .0f);
	DisappearRipple();
}

void AFloatingActor::SetDamping(float Linear, float Angular)
{
	Root->SetLinearDamping(Linear);
	Root->SetAngularDamping(Angular);
}

