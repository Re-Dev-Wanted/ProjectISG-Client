#include "MainPlayerCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Component/PlayerInventoryComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/GAS/Common/ISGAbilitySystemComponent.h"
#include "ProjectISG/GAS/Common/Attribute/ISGAttributeSet.h"
#include "ProjectISG/Systems/Logging/Component/ScreenShotComponent.h"

AMainPlayerCharacter::AMainPlayerCharacter()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArm->SetupAttachment(GetMesh());
	SpringArm->SetRelativeRotation({0, 90, 0});
	SpringArm->bUsePawnControlRotation = true;

	SpringArm->SetRelativeLocation({0, 0, 130});
	SpringArm->SetRelativeRotation({0, 0, 90});
	SpringArm->TargetArmLength = 360.f;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
	CameraComponent->SetupAttachment(SpringArm);

	CameraComponent->SetRelativeLocation({0, 0, 80});
	CameraComponent->SetRelativeRotation({0, -18, 0});

	PlayerInventoryComponent = CreateDefaultSubobject<
		UPlayerInventoryComponent>("Player Inventory Component");

	PlacementIndicatorComponent = CreateDefaultSubobject<UPlacementIndicatorComponent>("Placement Indicator Component");
	PlacementIndicatorComponent->Deactivate();
	ScreenShotComponent = CreateDefaultSubobject<UScreenShotComponent>(
		"ScreenShot Component");
}

void AMainPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (const APlayerController* PC = Cast<APlayerController>(
		GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
				PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AMainPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitializeAbilitySystem();

	InitializeInventorySystem();
}

void AMainPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitializeAbilitySystem();
}

void AMainPlayerCharacter::InitializeAbilitySystem()
{
	Super::InitializeAbilitySystem();

	if (const AMainPlayerState* PS = GetPlayerState<AMainPlayerState>())
	{
		AbilitySystemComponent = Cast<UISGAbilitySystemComponent>(
			PS->GetAbilitySystemComponent());

		AbilitySystemComponent->Initialize(InitializeData);

		AttributeSet = PS->GetAttributeSet();

		// 이후 Ability 시스템 관련 Delegate 연동 처리를 진행한다.
	}
}

void AMainPlayerCharacter::InitializeInventorySystem()
{
	PlayerInventoryComponent->Initialize();
}

void AMainPlayerCharacter::SetupPlayerInputComponent(
	UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<
		UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveInputAction,
		                                   ETriggerEvent::Triggered
		                                   , this, &ThisClass::MoveTo);
		EnhancedInputComponent->BindAction(LookInputAction,
		                                   ETriggerEvent::Triggered
		                                   , this, &ThisClass::Look);

		OnInputBindingNotified.Broadcast(EnhancedInputComponent);
	}
}

void AMainPlayerCharacter::MoveTo(const FInputActionValue& Value)
{
	const FVector2d InputVector = Value.Get<FVector2d>();

	const FRotator MoveRotation = {
		0, GetController()->GetControlRotation().Yaw, 0
	};
	const FVector ForwardVector = FRotationMatrix(MoveRotation).
		GetUnitAxis(EAxis::X) * InputVector.X;
	const FVector RightVector = FRotationMatrix(MoveRotation).
		GetUnitAxis(EAxis::Y) * InputVector.Y;

	const FVector MoveTo = ForwardVector + RightVector;

	AddMovementInput(MoveTo, 1);
}

void AMainPlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2d LookToValue = Value.Get<FVector2d>();
	AddControllerYawInput(LookToValue.X);
	AddControllerPitchInput(LookToValue.Y);
}
