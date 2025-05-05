
#include "BuoyancyComponent.h"

#include "ProjectISG/Systems/Water/Interfaces/BuoyantInterface.h"

UBuoyancyComponent::UBuoyancyComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
}

void UBuoyancyComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsInWater)
	{
		return;
	}

	if (!GetOwner())
	{
		return;
	}

	UPrimitiveComponent* Root = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());

	if (!Root || !Root->IsSimulatingPhysics())
	{
		return;
	}

	IBuoyantInterface* BuoyantInterface = Cast<IBuoyantInterface>(GetOwner());

	if (!BuoyantInterface)
	{
		return;
	}

	if (!BuoyantInterface->CanApplyBuoyancy())
	{
		return;
	}


	float ActorBottomZ = BuoyantInterface->GetActorBottomZ();
	
	float TargetZ = WaterLevel - SurfaceOffset; // 수면보다 약간 아래까지 올라오도록
	float Depth = TargetZ - ActorBottomZ;

	if (Depth > 0.f)
	{
		float Scale = BuoyantInterface->GetBuoyancyScale();
		float VelocityZ = Root->GetPhysicsLinearVelocity().Z;
		float UpwardResistance = FMath::Clamp(1.0f - VelocityZ / MaxRiseVelocity, 0.f, 1.f);
		float ForceZ = FMath::Clamp(Depth * BuoyancyStrength * Scale * UpwardResistance, 0.f, MaxBuoyancyForce);
		Root->AddForce(FVector(0, 0, ForceZ));
	}

}

