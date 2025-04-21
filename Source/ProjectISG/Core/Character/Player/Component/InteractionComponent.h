#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"


class AMainPlayerCharacter;
class UInputAction;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTISG_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractionComponent();

protected:
	virtual void BeginPlay() override;

	virtual void InitializeComponent() override;

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

	UFUNCTION()
	void BindingInputActions(
		UEnhancedInputComponent* EnhancedInputComponent);

	UFUNCTION()
	void OnInteractive();

	UFUNCTION()
	void OnTouch();
#pragma endregion

#pragma region Target
	UPROPERTY()
	TObjectPtr<AMainPlayerCharacter> PlayerCharacter;

	UPROPERTY(EditDefaultsOnly, Category = "Options|Targeting",
		meta = (AllowPrivateAccess = true))
	uint16 TargetRange = 300;

	UPROPERTY(EditDefaultsOnly, Category = "Options|Targeting",
		meta = (AllowPrivateAccess = true, ClampMin = "0", ClampMax = "100"))
	uint8 TargetRadius = 36;

	UPROPERTY()
	FHitResult TargetTraceResult;

	void LineTraceToFindTarget();
#pragma endregion
};
