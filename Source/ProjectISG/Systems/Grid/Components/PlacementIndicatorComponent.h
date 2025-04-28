#pragma once

#include "CoreMinimal.h"
#include "GridIndicatorComponent.h"
#include "Components/ActorComponent.h"
#include "ProjectISG/Systems/Grid/PlacementData.h"
#include "PlacementIndicatorComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTISG_API UPlacementIndicatorComponent : public UGridIndicatorComponent
{
	GENERATED_BODY()

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
	TEnumAsByte<ERotateDirection> RotateDirection = North;
	
	
	virtual void Execute() override;
	
	virtual void ExecuteInternal(FVector Pivot, FVector Location, FRotator Rotation, TSubclassOf<APlacement> PlacementClass, FItemMetaInfo ItemMetaInfo) override;

	UFUNCTION(BlueprintCallable)
	void Rotate(bool bClockwise);
};
