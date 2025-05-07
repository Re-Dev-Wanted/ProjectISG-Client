
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "BuoyancyComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTISG_API UBuoyancyComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	SETTER(float, WaterLevel)
	SETTER(bool, IsInWater)
	
	UBuoyancyComponent();

protected:
	float WaterLevel = 0.f;

	bool IsInWater = false;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BuoyancySettings)
	float MaxRiseVelocity = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BuoyancySettings)
	float MaxBuoyancyForce = 15000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BuoyancySettings)
	float BuoyancyStrength = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BuoyancySettings)
	float SurfaceOffset = 10.f;
};
