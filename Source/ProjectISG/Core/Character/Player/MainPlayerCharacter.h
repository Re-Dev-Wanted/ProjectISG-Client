#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/Character/BaseCharacter.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "MainPlayerCharacter.generated.h"

class UPlayerSoundComponent;
class UInputActionComponent;
class UDiaryComponent;
class UInteractionComponent;
class UScreenShotComponent;
class UPlayerInventoryComponent;
class UPlacementIndicatorComponent;
class UPlayerHandSlotComponent;

class USpringArmComponent;
class UCameraComponent;

struct FInputActionValue;
class UInputAction;
class UInputComponent;
class UInputMappingContext;
class UMediaSoundComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInputBindingNotified
                                            , UEnhancedInputComponent*
                                            , EnhancedInputComponent);

UCLASS()
class PROJECTISG_API AMainPlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AMainPlayerCharacter();

	FOnInputBindingNotified OnInputBindingNotified;

	GETTER(TObjectPtr<UCameraComponent>, CameraComponent);

	GETTER(TObjectPtr<UInputMappingContext>, DefaultMappingContext)

	GETTER(TObjectPtr<UScreenShotComponent>, ScreenShotComponent)

	GETTER(TObjectPtr<UPlayerSoundComponent>, PlayerSoundComponent)

protected:
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(
		UInputComponent* PlayerInputComponent) override;

	virtual void OnRep_PlayerState() override;
	void InitializeInternal();

	virtual void PossessedBy(AController* NewController) override;

	virtual void InitializeAbilitySystem() override;

	virtual void GetLifetimeReplicatedProps(
		TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	GETTER(TObjectPtr<UMediaSoundComponent>, MediaSoundComponent)
	GETTER_SETTER(bool, bIsSleep)
	GETTER_SETTER(bool, bLieInBed)
	GETTER(TObjectPtr<UPlacementIndicatorComponent>
	       , PlacementIndicatorComponent)
	GETTER(TObjectPtr<UDiaryComponent>, DiaryComponent)
	GETTER(TObjectPtr<UPlayerInventoryComponent>, PlayerInventoryComponent)
	GETTER(TObjectPtr<UInteractionComponent>, InteractionComponent)
	GETTER(TObjectPtr<UPlayerHandSlotComponent>, HandSlotComponent)
	GETTER(TObjectPtr<UCameraComponent>, ScreenShotCameraComponent)

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_SetActorTransformReplicated(const FTransform& Transform);

private:
#pragma region ActorComponent
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UDiaryComponent> DiaryComponent;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UPlayerInventoryComponent> PlayerInventoryComponent;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputActionComponent> InputActionComponent;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UScreenShotComponent> ScreenShotComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly
		, meta = (AllowPrivateAccess = true))
	TObjectPtr<UPlacementIndicatorComponent> PlacementIndicatorComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly
		, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInteractionComponent> InteractionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly
		, meta = (AllowPrivateAccess = true))
	TObjectPtr<UPlayerHandSlotComponent> HandSlotComponent;
#pragma endregion

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USpringArmComponent> ScreenShotSpringArm;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UCameraComponent> ScreenShotCameraComponent;

	UPROPERTY()
	TObjectPtr<UMediaSoundComponent> MediaSoundComponent;

	UPROPERTY()
	TObjectPtr<UPlayerSoundComponent> PlayerSoundComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Options|Input"
		, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Options|Input"
		, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> MoveInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Options|Input"
		, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> LookInputAction;

	UFUNCTION()
	void MoveTo(const FInputActionValue& Value);

	UFUNCTION()
	void Look(const FInputActionValue& Value);

	// 장진혁
#pragma region JJH
	UPROPERTY(Replicated, EditAnywhere, Category = "Sleep"
		, meta = (AllowPrivateAccess = true))
	bool bIsSleep = false;

	UPROPERTY(Replicated, EditAnywhere, Category = "Sleep"
		, meta = (AllowPrivateAccess = true))
	bool bLieInBed = false;
#pragma endregion
};
