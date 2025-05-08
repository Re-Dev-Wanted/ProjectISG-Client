#include "Bobber.h"

#include "Components/BoxComponent.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
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

	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();

	TWeakObjectPtr<ABobber> WeakThis = this;

	Streamable.RequestAsyncLoad
	(
		Fish.ToSoftObjectPath(), 
		FStreamableDelegate::CreateLambda
		(
			[WeakThis, Fish]()
			{
				if (WeakThis.IsValid() && Fish.IsValid())
				{
					USkeletalMesh* LoadedMesh = Fish.Get();
					if (LoadedMesh)
					{
						WeakThis->FishMesh->SetSkeletalMesh(LoadedMesh);
					}
				}
			}
		)
	);
}

void ABobber::RemoveFish()
{
	if (FishMesh)
	{
		FishMesh->SetSkeletalMesh(nullptr);
	}
}

float ABobber::GetBuoyancyScale() const
{
	return 0.5f;
}
