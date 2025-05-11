#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Components/ActorComponent.h"
#include "ProjectISG/Systems/Grid/PlacementData.h"
#include "ProjectISG/Utils/MacroUtil.h"
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

	void SetIsActive(bool NewActive);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = IndicatorProperties)
	float InterpSpeed = 5.f;

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
	TObjectPtr<class AMainPlayerCharacter> Player = nullptr; 

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

	UPROPERTY(EditDefaultsOnly, Category = "Options|Targeting",
		meta = (AllowPrivateAccess = true))
	uint16 TargetRange = 300;

	UPROPERTY(EditDefaultsOnly, Category = "Options|Targeting",
		meta = (AllowPrivateAccess = true, ClampMin = "0", ClampMax = "100"))
	uint8 TargetRadius = 36;

	UPROPERTY()
	FHitResult TargetTraceResult;

	bool bIsIndicatorActive = false;

	bool bIsInfiniteItem = false;
	
	bool bIsBlocked = false;

	bool IsActive = true;

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

	void LineTrace();
};
