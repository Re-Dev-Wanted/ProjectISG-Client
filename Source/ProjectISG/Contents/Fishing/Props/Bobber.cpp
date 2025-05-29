#include "Bobber.h"

#include "Components/BoxComponent.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Kismet/GameplayStatics.h"

ABobber::ABobber()
{
	LineAttachPoint = CreateDefaultSubobject<USceneComponent>(TEXT("LineAttachPoint"));
	LineAttachPoint->SetupAttachment(MeshComponent);

	FishMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FishMesh"));
	FishMeshComp->SetupAttachment(RootComponent);
	FishMeshComp->SetRelativeScale3D(FVector::OneVector * 0.5f);

	Root->SetSimulatePhysics(false);
}

void ABobber::SuggestProjectileVelocity(const FVector& StartLocation, const FVector& EndLocation)
{
	FVector Velocity;

	SetCollisionAndPhysicsEnabled(true);
	
	bool bSuggest = UGameplayStatics::SuggestProjectileVelocity_CustomArc(
		this,
		Velocity,
		GetActorLocation(),
		EndLocation,
		GetWorld()->GetGravityZ(),
		0.5f
	);
	
	Root->SetPhysicsLinearVelocity(Velocity);
	
	// if (bSuggest)
	// {
	// 	FPredictProjectilePathParams PredictParams(10.0f, StartLocation, Velocity, 5.0f);
	// 	PredictParams.DrawDebugTime = 5.0f;     
	// 	PredictParams.DrawDebugType = EDrawDebugTrace::Type::ForDuration;
	// 	PredictParams.OverrideGravityZ = GetWorld()->GetGravityZ();
	// 	FPredictProjectilePathResult result;
	// 	UGameplayStatics::PredictProjectilePath(this, PredictParams, result);
	// }
}

void ABobber::OnBite(UStaticMesh* Fish)
{
	if (!Root->IsSimulatingPhysics())
	{
		return;
	}

	FVector Impulse = FVector::DownVector * ImpulseStrength;
	Root->AddImpulse(Impulse);

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), SFX_Bite, GetActorLocation());

	FishMeshComp->SetStaticMesh(Fish);
	FTransform Pivot = FishMeshComp->GetSocketTransform(TEXT("BiteSocket"), RTS_Component);
	FishMeshComp->SetRelativeLocation(-(Pivot.GetLocation() * 0.5f));
}

void ABobber::RemoveFish()
{
	if (FishMeshComp)
	{
		FishMeshComp->SetStaticMesh(nullptr);
	}
}

float ABobber::GetBuoyancyScale() const
{
	return 0.5f;
}
