#pragma once

#include "CoreMinimal.h"
#include "GridIndicatorComponent.h"
#include "InputActionValue.h"
#include "Components/ActorComponent.h"
#include "ProjectISG/Systems/Grid/PlacementData.h"
#include "PlacementIndicatorComponent.generated.h"

class UInputAction;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTISG_API UPlacementIndicatorComponent : public UGridIndicatorComponent
{
	GENERATED_BODY()

public:
	UPlacementIndicatorComponent();
	
	virtual void InitializeComponent() override;
	
	virtual void Execute() override;

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual void ExecuteInternal(FVector Pivot, FVector Location, FRotator Rotation, TSubclassOf<APlacement> PlacementClass, FItemMetaInfo ItemMetaInfo) override;

	UPROPERTY()
	TEnumAsByte<ERotateDirection> RotateDirection = North;

	UPROPERTY(EditDefaultsOnly, Category = "Options|Input",
		meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> RotateAction;

	UPROPERTY(EditDefaultsOnly, Category = "Options|Input",
		meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> TouchAction;

	UFUNCTION()
	void BindingInputActions(
		UEnhancedInputComponent* EnhancedInputComponent);
	
	UFUNCTION()
	void OnRotate(const FInputActionValue& InputActionValue);
};
