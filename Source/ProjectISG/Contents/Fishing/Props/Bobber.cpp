#include "Bobber.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

ABobber::ABobber()
{
	PrimaryActorTick.bCanEverTick = true;
	
	LineAttachPoint = CreateDefaultSubobject<USceneComponent>(TEXT("LineAttachPoint"));
	LineAttachPoint->SetupAttachment(MeshComponent);

	FishMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FishMesh"));
	FishMeshComp->SetupAttachment(RootComponent);
	FishMeshComp->SetRelativeScale3D(FVector::OneVector * 0.5f);

	Ripples = CreateDefaultSubobject<UChildActorComponent>(TEXT("Ripple"));
	Ripples->SetupAttachment(RootComponent);

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

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Effect_Bite, GetActorLocation());
	
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), SFX_Bite, GetActorLocation());

	// FishMeshComp->SetStaticMesh(Fish);
	// FTransform Pivot = FishMeshComp->GetSocketTransform(TEXT("BiteSocket"), RTS_Component);
	// FishMeshComp->SetRelativeLocation(-(Pivot.GetLocation() * 0.5f));
}

void ABobber::RemoveFish()
{
	// if (FishMeshComp)
	// {
	// 	FishMeshComp->SetStaticMesh(nullptr);
	// }
}

void ABobber::AppearRipple(FVector Location, FRotator Rotation)
{
	Ripples->SetHiddenInGame(false);
}

void ABobber::DisappearRipple()
{
	Ripples->SetHiddenInGame(true);
}

float ABobber::GetBuoyancyScale() const
{
	return 0.5f;
}

void ABobber::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!Ripples)
	{
		return;
	}
	
	Ripples->SetWorldRotation(FRotator::ZeroRotator);
}
