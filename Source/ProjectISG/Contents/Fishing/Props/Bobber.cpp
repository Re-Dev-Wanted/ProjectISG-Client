#include "Bobber.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

ABobber::ABobber()
{
	LineAttachPoint = CreateDefaultSubobject<USceneComponent>(TEXT("LineAttachPoint"));
	LineAttachPoint->SetupAttachment(RootComponent);

	FishMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FishMesh"));
	FishMesh->SetupAttachment(RootComponent);

	Root->SetSimulatePhysics(false);
}

void ABobber::SuggestProjectileVelocity(const FVector& StartLocation, const FVector& EndLocation)
{
	FVector Velocity;

	SetCollisionAndPhysicsEnabled(true);
	
	bool bSuggest = UGameplayStatics::SuggestProjectileVelocity_CustomArc(
		this,
		Velocity,
		StartLocation,
		EndLocation,
		GetWorld()->GetGravityZ(),
		0.75f
	);

	if (bSuggest)
	{
		FPredictProjectilePathParams PredictParams(10.0f, StartLocation, Velocity, 5.0f);
		PredictParams.DrawDebugTime = 5.0f;     
		PredictParams.DrawDebugType = EDrawDebugTrace::Type::ForDuration;
		PredictParams.OverrideGravityZ = GetWorld()->GetGravityZ();
		FPredictProjectilePathResult result;
		UGameplayStatics::PredictProjectilePath(this, PredictParams, result);
	}
}

void ABobber::OnBite(TSoftObjectPtr<USkeletalMesh> Fish)
{
	if (!Root->IsSimulatingPhysics())
	{
		return;
	}

	FVector Impulse = FVector::DownVector * ImpulseStrength;
	Root->AddImpulse(Impulse);

	FishMesh->SetSkeletalMesh(Fish.Get());
}

void ABobber::RemoveFish()
{
	if (FishMesh)
	{
		FishMesh->SetSkeletalMesh(nullptr);
	}
}
