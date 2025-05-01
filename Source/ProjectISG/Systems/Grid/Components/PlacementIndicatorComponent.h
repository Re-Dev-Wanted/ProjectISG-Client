#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Components/ActorComponent.h"
#include "ProjectISG/Systems/Grid/PlacementData.h"
#include "PlacementIndicatorComponent.generated.h"

class UInputAction;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTISG_API UPlacementIndicatorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlacementIndicatorComponent();
	
	virtual void InitializeComponent() override;
	
	void Execute();
	
	UFUNCTION()
	void OnChange(uint16 ItemId);

	UFUNCTION()
	void OnActivate(const TSubclassOf<class APlacement>& Factory);

	UFUNCTION()
	void OnDeactivate();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = IndicatorProperties)
	float InterpSpeed = 5.f;
	
protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
	TObjectPtr<class APlacement> IndicateActor = nullptr;
	
	UPROPERTY()
	TObjectPtr<class APlayerController> PlayerController = nullptr;

	UPROPERTY()
	TEnumAsByte<ERotateDirection> RotateDirection = North;

	UPROPERTY(EditDefaultsOnly, Category = "Options|Input",
		meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> RotateAction;

	UPROPERTY(EditDefaultsOnly, Category = "Options|Input",
		meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> TouchAction;

	bool bIsIndicatorActive = false;

	bool bIsInfiniteItem = false;

	uint16 PlacementItemId = 0;

	UFUNCTION()
	void BindingInputActions(
		UEnhancedInputComponent* EnhancedInputComponent);
	
	UFUNCTION()
	void OnRotate(const FInputActionValue& InputActionValue);

	UFUNCTION(Server, Reliable)
	void Server_Execute(FVector Pivot, FVector Location, FRotator Rotation, 
	TSubclassOf<APlacement> PlacementClass, uint16 ItemId);

	UFUNCTION()
	void ExecuteInternal(FVector Pivot, FVector Location, FRotator Rotation, TSubclassOf<APlacement> PlacementClass, uint16 ItemId);
};
