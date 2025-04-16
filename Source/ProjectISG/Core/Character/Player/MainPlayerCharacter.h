#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/Character/BaseCharacter.h"
#include "MainPlayerCharacter.generated.h"

struct FInputActionValue;

class USpringArmComponent;
class UCameraComponent;

class UInputAction;
class UInputComponent;
class UInputMappingContext;

UCLASS()
class PROJECTISG_API AMainPlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AMainPlayerCharacter();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(
		UInputComponent* PlayerInputComponent) override;

	virtual void OnRep_PlayerState() override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void InitializeAbilitySystem() override;

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
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
};
