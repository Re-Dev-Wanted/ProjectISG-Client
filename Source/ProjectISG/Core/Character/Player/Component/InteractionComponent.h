#pragma once

#include "CoreMinimal.h"
#include "Base/MainPlayerComponent.h"
#include "InteractionComponent.generated.h"


class UInputAction;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTISG_API UInteractionComponent : public UMainPlayerComponent
{
	GENERATED_BODY()

public:
	UInteractionComponent();

protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType,
	                           FActorComponentTickFunction*
	                           ThisTickFunction) override;

private:
#pragma region Input
	UPROPERTY(EditDefaultsOnly, Category = "Options|Input",
		meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> InteractionAction;

	UPROPERTY(EditDefaultsOnly, Category = "Options|Input",
		meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> TouchAction;

	virtual void BindingInputActions(
		UEnhancedInputComponent* EnhancedInputComponent) override;

	UFUNCTION()
	void OnInteractive();

	UFUNCTION()
	void OnTouch();
#pragma endregion

#pragma region Target
	UPROPERTY(EditDefaultsOnly, Category = "Options|Targeting",
		meta = (AllowPrivateAccess = true))
	uint16 TargetRange = 300;

	UPROPERTY(EditDefaultsOnly, Category = "Options|Targeting",
		meta = (AllowPrivateAccess = true))
	uint16 TargetRadius = 36;

	UPROPERTY(EditDefaultsOnly, Category = "Options|Targeting",
		meta = (AllowPrivateAccess = true, ClampMin = "0", ClampMax = "100"))
	uint8 TargetHeight = 8;

	UPROPERTY()
	FHitResult TargetTraceResult;

	void LineTraceToFindTarget();
#pragma endregion
};
