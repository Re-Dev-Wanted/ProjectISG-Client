#include "Bobber.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

ABobber::ABobber()
{
	LineAttachPoint = CreateDefaultSubobject<USceneComponent>(TEXT("LineAttachPoint"));
	LineAttachPoint->SetupAttachment(RootComponent);

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
		0.3f
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
