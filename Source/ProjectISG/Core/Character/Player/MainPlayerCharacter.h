#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/Character/BaseCharacter.h"
#include "ProjectISG/Systems/Grid/Components/PlacementIndicatorComponent.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "MainPlayerCharacter.generated.h"


class UScreenShotComponent;
class UPlayerInventoryComponent;
struct FInputActionValue;

class USpringArmComponent;
class UCameraComponent;

class UInputAction;
class UInputComponent;
class UInputMappingContext;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInputBindingNotified,
                                            UEnhancedInputComponent*,
                                            EnhancedInputComponent);

UCLASS()
class PROJECTISG_API AMainPlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AMainPlayerCharacter();

	FOnInputBindingNotified OnInputBindingNotified;

protected:
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(
		UInputComponent* PlayerInputComponent) override;

	virtual void OnRep_PlayerState() override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void InitializeAbilitySystem() override;

	void InitializeInventorySystem();

public:
	GETTER_SETTER(bool, bIsSleep);
	GETTER_SETTER(bool, bLieOnBed);
	GETTER_SETTER(TObjectPtr<AActor>, MainHandItem)
	GETTER(TObjectPtr<UPlacementIndicatorComponent>,
	       PlacementIndicatorComponent)
	GETTER(TObjectPtr<UPlayerInventoryComponent>, PlayerInventoryComponent)

private:
	UPROPERTY()
	TObjectPtr<AActor> MainHandItem;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UPlayerInventoryComponent> PlayerInventoryComponent;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UScreenShotComponent> ScreenShotComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
		meta = (AllowPrivateAccess = true))
	TObjectPtr<UPlacementIndicatorComponent> PlacementIndicatorComponent;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Options|Input",
		meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Options|Input",
		meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> MoveInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Options|Input",
		meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> LookInputAction;

	UFUNCTION()
	void MoveTo(const FInputActionValue& Value);

	UFUNCTION()
	void Look(const FInputActionValue& Value);

	// 장진혁
	UPROPERTY(EditAnywhere, Category = "Sleep",
		meta = (AllowPrivateAccess = true))
	bool bIsSleep = false;
	UPROPERTY(EditAnywhere, Category = "Sleep",
		meta = (AllowPrivateAccess = true))
	bool bLieOnBed = false;
};
