#include "InteractionComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "EnhancedInputComponent.h"
#include "ProjectISG/Systems/Input/Interface/InteractionInterface.h"


UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInteractionComponent::BindingInputActions(
	UEnhancedInputComponent* EnhancedInputComponent)
{
	Super::BindingInputActions(EnhancedInputComponent);

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

	Interaction->OnInteractive();
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

	Interaction->OnTouch();
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
	const FVector OwnerStartLocation = GetOwner()->GetActorLocation();
	const FVector OwnerEndLocation = OwnerStartLocation + GetOwner()->
		GetActorForwardVector() * TargetRange;

	UKismetSystemLibrary::CapsuleTraceSingle(GetWorld(),
	                                         OwnerStartLocation,
	                                         OwnerEndLocation, TargetRadius,
	                                         TargetHeight, TraceTypeQuery1,
	                                         false, IgnoreActors,
	                                         EDrawDebugTrace::ForOneFrame,
	                                         TargetTraceResult, true);
}
