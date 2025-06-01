#include "WaterVolume.h"

#include "Components/BoxComponent.h"

#include "ProjectISG/Systems/Water/Actors/FloatingActor.h"
#include "ProjectISG/Systems/Water/Components/BuoyancyComponent.h"

AWaterVolume::AWaterVolume()
{
	VolumeCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	SetRootComponent(VolumeCollision);
	VolumeCollision->SetCollisionResponseToAllChannels(ECR_Overlap);
	VolumeCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	WaterSurface = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Surface"));
	WaterSurface->SetupAttachment(VolumeCollision);
	WaterSurface->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	VolumeCollision->OnComponentBeginOverlap.AddDynamic(this, &AWaterVolume::OnBeginOverlap);
	VolumeCollision->OnComponentEndOverlap.AddDynamic(this, &AWaterVolume::OnEndOverlap);
}

void AWaterVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWaterVolume::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	float SurfaceZ = GetActorLocation().Z + VolumeCollision->GetScaledBoxExtent().Z;

	for (AFloatingActor* Actor : FloatingActors)
	{
		if (!Actor)
		{
			continue;
		}

		if (UBuoyancyComponent* BuoyancyComponent = Actor->GetBuoyancyComponent())
		{
			BuoyancyComponent->SetWaterLevel(SurfaceZ);
		}
	}
}

void AWaterVolume::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AFloatingActor* Actor = Cast<AFloatingActor>(OtherActor))
	{
		FloatingActors.Add(Actor);
		Actor->EnterWater();
		Actor->SetDamping(10.f, 3.0f);
		Actor->AppearRipple(FVector::ZeroVector, FRotator::ZeroRotator);
	}
}

void AWaterVolume::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AFloatingActor* Actor = Cast<AFloatingActor>(OtherActor))
	{
		Actor->GetBuoyancyComponent()->SetIsInWater(false);
		Actor->SetDamping(0.01f, .0f);
		FloatingActors.Remove(Actor);
	}
}
