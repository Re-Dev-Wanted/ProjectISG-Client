#include "InteractionComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "ProjectISG/Contents/Trading/TradingNPC.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/UI/Gameplay/MainHUD/UI/UIC_MainHUD.h"
#include "ProjectISG/Systems/Input/Interface/InteractionInterface.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"

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

	OwnerPlayer->OnUpdateSelectedItem.AddDynamic(this, &ThisClass::OnChange);
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

void UInteractionComponent::OnChange(uint16 ItemId)
{
	const FItemInfoData ItemInfoData = UItemManager::GetItemInfoById(ItemId);

	const bool bIsStructure = ItemInfoData.GetItemType() !=
		EItemType::Equipment && UItemManager::IsItemCanHousing(ItemId);

	SetIsInteractive(!bIsStructure);

	ToggleInteractiveUI();
}

void UInteractionComponent::OnInteractive()
{
	if (!IsInteractive)
	{
		return;
	}
	
	if (!TargetTraceResult.GetActor())
	{
		return;
	}

	ABaseInteractiveActor* InteractActor = Cast<ABaseInteractiveActor>(
		TargetTraceResult.GetActor());

	if (InteractActor && InteractActor->GetCanInteractive())
	{
		InteractActor->OnInteractive(GetOwner());
		return;
	}

	IInteractionInterface* InteractionInterface = Cast<IInteractionInterface>(TargetTraceResult.GetActor());

	if (!InteractionInterface || !InteractionInterface->GetCanInteractive())
	{
		return;
	}

	InteractionInterface->OnInteractive(GetOwner());
}

void UInteractionComponent::OnTouch()
{
	if (!IsInteractive)
	{
		return;
	}
	
	if (!TargetTraceResult.GetActor())
	{
		return;
	}

	ABaseInteractiveActor* InteractActor = Cast<ABaseInteractiveActor>(
		TargetTraceResult.GetActor());

	if (InteractActor && InteractActor->GetCanTouch())
	{
		InteractActor->OnTouch(GetOwner());
		return;
	}

	IInteractionInterface* InteractionInterface = Cast<IInteractionInterface>(TargetTraceResult.GetActor());

	if (!InteractionInterface || !InteractionInterface->GetCanTouch())
	{
		return;
	}

	InteractionInterface->OnTouch(GetOwner());
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

void UInteractionComponent::ToggleInteractiveUI()
{
	const AMainPlayerController* PC = PlayerCharacter->GetController<
		AMainPlayerController>();
	
	if (!IsInteractive || !TargetTraceResult.GetActor())
	{
		if (PC->GetMainHUD())
		{
			PC->GetMainHUD()->ToggleInteractiveUI(false);
		}

		return;
	}

	const IInteractionInterface* Interaction = Cast<IInteractionInterface>(
		TargetTraceResult.GetActor());

	if (!Interaction || (!Interaction->GetCanInteractive() && !Interaction->GetCanTouch()))
	{
		if (PC->GetMainHUD())
		{
			PC->GetMainHUD()->ToggleInteractiveUI(false);
		}

		return;
	}
		
	if (Interaction->GetCanInteractive())
	{
		if (PC->GetMainHUD())
		{
			PC->GetMainHUD()->ToggleInteractiveUI(
				TEXT("F"), Interaction->GetInteractiveDisplayText());
		}
	}
	
	if (Interaction->GetCanTouch())
	{
		if (PC->GetMainHUD())
		{
			if (!Interaction->GetCanInteractive())
			{
				PC->GetMainHUD()->ToggleInteractiveUI(
					TEXT("RM"), Interaction->GetTouchDisplayText(GetOwner()));
			}
			else
			{
				PC->GetMainHUD()->AdditiveToggleInteractiveUI(
					TEXT("RM"), Interaction->GetTouchDisplayText(GetOwner()));
			}
		}
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
		ToggleInteractiveUI();
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

void UInteractionComponent::Server_OnInteractiveResponse_Implementation(
	class AActor* Causer)
{
	IInteractionInterface* Interaction = Cast<IInteractionInterface>(
		TargetTraceResult.GetActor());

	if (Interaction)
	{
		Interaction->OnInteractiveResponse(Causer);
	}
}

void UInteractionComponent::Server_OnTouchResponse_Implementation(
	class AActor* Causer)
{
	IInteractionInterface* Interaction = Cast<IInteractionInterface>(
		TargetTraceResult.GetActor());

	if (Interaction)
	{
		Interaction->OnTouchResponse(Causer);
	}
}

void UInteractionComponent::Server_Interact_Implementation(
	class ABaseInteractiveActor*
	InteractActor, AActor* Causer)
{
	// 정상 처리
	InteractActor->OnInteractive(Causer);
}

void UInteractionComponent::Server_Touch_Implementation(
	class ABaseInteractiveActor*
	InteractActor, AActor* Causer)
{
	InteractActor->OnTouch(Causer);
}
