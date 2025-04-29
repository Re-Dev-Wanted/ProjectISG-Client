#include "InteractionComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "ProjectISG/Contents/Farming/BaseCrop.h"
#include "ProjectISG/Contents/Trading/TradingNPC.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/UI/Gameplay/MainHUD/UI/UIC_MainHUD.h"
#include "ProjectISG/Systems/Input/Interface/InteractionInterface.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	IsInteractive = true;
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

	if (IsInteractive)
	{
		LineTraceToFindTarget();
	}
}

void UInteractionComponent::LineTraceToFindTarget()
{
	const TArray<AActor*> IgnoreActors;
	const FVector OwnerStartLocation = PlayerCharacter->GetActorLocation();
	const FVector OwnerEndLocation = OwnerStartLocation + PlayerCharacter->
		GetCameraComponent()->GetForwardVector() * TargetRange;

	const bool IsSuccess = UKismetSystemLibrary::CapsuleTraceSingle(GetWorld(),
		OwnerStartLocation,
		OwnerEndLocation, TargetRadius,
		TargetRadius, TraceTypeQuery1,
		false, IgnoreActors,
		EDrawDebugTrace::ForOneFrame,
		TargetTraceResult, true);

	const AMainPlayerController* PC = PlayerCharacter->GetController<
		AMainPlayerController>();
	if (!PC)
	{
		return;
	}

	if (IsSuccess)
	{
		if (!TargetTraceResult.GetActor())
		{
			return;
		}

		const IInteractionInterface* Interaction = Cast<IInteractionInterface>(
			TargetTraceResult.GetActor());

		if (!Interaction)
		{
			if (PC->GetMainHUD())
			{
				PC->GetMainHUD()->ToggleInteractiveUI(false);
			}

			return;
		}

		// TODO: 현재는 Interaction과 Touch 둘다 사용하는 UI는 없다
		// 추가 개발 필요함.
		if (Interaction->GetCanInteractive())
		{
			if (PC->GetMainHUD())
			{
				PC->GetMainHUD()->ToggleInteractiveUI(
					TEXT("F"), Interaction->GetDisplayText());
			}
		}

		if (Interaction->GetCanTouch())
		{
			if (PC->GetMainHUD())
			{
				PC->GetMainHUD()->ToggleInteractiveUI(
					TEXT("LM"), Interaction->GetDisplayText());
			}
		}
	}
	else
	{
		if (PC->GetMainHUD())
		{
			PC->GetMainHUD()->ToggleInteractiveUI(false);
		}
	}
}

void UInteractionComponent::SetIsInteractive(const bool NewIsInteractive)
{
	IsInteractive = NewIsInteractive;

	if (!NewIsInteractive)
	{
		if (!PlayerCharacter)
		{
			return;
		}

		const AMainPlayerController* PC = PlayerCharacter->GetController<
			AMainPlayerController>();

		if (!PC)
		{
			return;
		}

		if (!PC->GetMainHUD())
		{
			return;
		}

		PC->GetMainHUD()->ToggleInteractiveUI(false);
	}
}

void UInteractionComponent::Server_OnInteractiveResponse_Implementation()
{
	IInteractionInterface* Interaction = Cast<IInteractionInterface>(
		TargetTraceResult.GetActor());

	if (Interaction)
	{
		Interaction->OnInteractiveResponse();
	}
}

