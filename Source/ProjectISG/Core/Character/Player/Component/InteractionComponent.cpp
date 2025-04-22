#include "InteractionComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Systems/Input/Interface/InteractionInterface.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<AMainPlayerCharacter>(GetOwner());
}

void UInteractionComponent::InitializeComponent()
{
	Super::InitializeComponent();

	AMainPlayerCharacter* OwnerPlayer = Cast<AMainPlayerCharacter>(GetOwner());

	OwnerPlayer->OnInputBindingNotified.AddDynamic(
		this, &ThisClass::BindingInputActions);
}

void UInteractionComponent::BindingInputActions(
	UEnhancedInputComponent* EnhancedInputComponent)
{
	EnhancedInputComponent->BindAction(InteractionAction,
	                                   ETriggerEvent::Triggered, this,
	                                   &ThisClass::OnInteractive);
	EnhancedInputComponent->BindAction(TouchAction,
	                                   ETriggerEvent::Triggered, this,
	                                   &ThisClass::OnTouch);
}

void UInteractionComponent::OnInteractive()
{
	if (!TargetTraceResult.GetActor())
	{
		return;
	}

	IInteractionInterface* Interaction = Cast<IInteractionInterface>(
		TargetTraceResult.GetActor());

	if (!Interaction)
	{
		return;
	}

	Interaction->OnInteractive(GetOwner());
}

void UInteractionComponent::OnTouch()
{
	if (!TargetTraceResult.GetActor())
	{
		return;
	}

	IInteractionInterface* Interaction = Cast<IInteractionInterface>(
		TargetTraceResult.GetActor());

	if (!Interaction)
	{
		return;
	}

	Interaction->OnTouch(GetOwner());
}

void UInteractionComponent::TickComponent(float DeltaTime,
                                          enum ELevelTick TickType,
                                          FActorComponentTickFunction*
                                          ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	LineTraceToFindTarget();
}

void UInteractionComponent::LineTraceToFindTarget()
{
	const TArray<AActor*> IgnoreActors;
	const FVector OwnerStartLocation = PlayerCharacter->GetActorLocation();
	const FVector OwnerEndLocation = OwnerStartLocation + PlayerCharacter->
		GetCameraComponent()->GetForwardVector() * TargetRange;

	UKismetSystemLibrary::CapsuleTraceSingle(GetWorld(),
	                                         OwnerStartLocation,
	                                         OwnerEndLocation, TargetRadius,
	                                         TargetRadius, TraceTypeQuery1,
	                                         false, IgnoreActors,
	                                         EDrawDebugTrace::ForOneFrame,
	                                         TargetTraceResult, true);
}
