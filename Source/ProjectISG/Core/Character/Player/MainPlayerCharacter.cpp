#include "MainPlayerCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Component/InputActionComponent.h"
#include "Component/InteractionComponent.h"
#include "Component/PlayerHandSlotComponent.h"
#include "Component/PlayerInventoryComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "ProjectISG/Contents/Diary/Component/DiaryComponent.h"
#include "ProjectISG/Core/ISGGameInstance.h"
#include "ProjectISG/Core/GameMode/MainGameState.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/GAS/Common/ISGAbilitySystemComponent.h"
#include "ProjectISG/GAS/Common/Attribute/ISGAttributeSet.h"
#include "ProjectISG/Systems/Grid/Components/PlacementIndicatorComponent.h"
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
	CameraComponent->SetRelativeRotation({0, 0, -18});

	ScreenShotSpringArm = CreateDefaultSubobject<USpringArmComponent>(
		"ScreenShot Spring Arm");
	ScreenShotSpringArm->SetupAttachment(GetMesh());

	ScreenShotCameraComponent = CreateDefaultSubobject<UCameraComponent>(
		"ScreenShot Camera");
	ScreenShotCameraComponent->SetupAttachment(ScreenShotSpringArm);

	PlayerInventoryComponent = CreateDefaultSubobject<
		UPlayerInventoryComponent>("Player Inventory Component");

	PlacementIndicatorComponent = CreateDefaultSubobject<
		UPlacementIndicatorComponent>("Placement Indicator Component");
	PlacementIndicatorComponent->Deactivate();

	ScreenShotComponent = CreateDefaultSubobject<UScreenShotComponent>(
		"ScreenShot Component");

	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(
		"Interaction Component");

	HandSlotComponent = CreateDefaultSubobject<UPlayerHandSlotComponent>
		("Hand Slot Component");

	InputActionComponent = CreateDefaultSubobject<UInputActionComponent>
		("Input Action Component");

	DiaryComponent = CreateDefaultSubobject<
		UDiaryComponent>("Diary Component");
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

	if (HasAuthority() && IsLocallyControlled())
	{
		FString SessionId = GetWorld()->GetGameInstance<UISGGameInstance>()->
		                                GetSessionId();
		GetWorld()->GetGameState<AMainGameState>()->SetSessionId(SessionId);
		UE_LOG(LogTemp, Warning, TEXT("세션 복사 : %s"), *SessionId);
	}
}

void AMainPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitializeAbilitySystem();

	PlayerInventoryComponent->InitializePlayerInventory();
	HandSlotComponent->InitializePlayerHandSlot();

	if (IsLocallyControlled())
	{
		GetPlayerState<AMainPlayerState>()->InitializeData();
	}
}

void AMainPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitializeAbilitySystem();

	PlayerInventoryComponent->InitializePlayerInventory();
	HandSlotComponent->InitializePlayerHandSlot();

	if (IsLocallyControlled())
	{
		GetPlayerState<AMainPlayerState>()->InitializeData();
	}
}

void AMainPlayerCharacter::InitializeAbilitySystem()
{
	Super::InitializeAbilitySystem();

	if (AMainPlayerState* PS = GetPlayerState<AMainPlayerState>())
	{
		AbilitySystemComponent = Cast<UISGAbilitySystemComponent>(
			PS->GetAbilitySystemComponent());

		AbilitySystemComponent->InitAbilityActorInfo(PS, this);
		AbilitySystemComponent->Initialize(InitializeData);

		AttributeSet = PS->GetAttributeSet();

		// 이후 Ability 시스템 관련 Delegate 연동 처리를 진행한다.
	}
}

void AMainPlayerCharacter::GetLifetimeReplicatedProps(
	TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, bIsSleep);
	DOREPLIFETIME(ThisClass, bLieInBed);
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
	const FVector2d MovementVector = Value.Get<FVector2d>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).
			GetUnitAxis(EAxis::X);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(
			EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.X);
		AddMovementInput(RightDirection, MovementVector.Y);
	}
}

void AMainPlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2d LookToValue = Value.Get<FVector2d>();
	AddControllerYawInput(LookToValue.X);
	AddControllerPitchInput(LookToValue.Y);
}

void AMainPlayerCharacter::Server_SetActorTransformReplicated_Implementation(
	const FTransform& Transform)
{
	SetActorTransform(Transform);
}
