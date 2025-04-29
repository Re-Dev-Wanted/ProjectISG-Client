#include "PlacementIndicatorComponent.h"

#include "EnhancedInputComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/PlayerInventoryComponent.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Systems/Grid/Actors/Placement.h"
#include "ProjectISG/Systems/Grid/Manager/GridManager.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"

class AMainPlayerCharacter;

UPlacementIndicatorComponent::UPlacementIndicatorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	
	ConstructorHelpers::FObjectFinder<UInputAction> InputAction(TEXT("/Game/Core/Character/Blueprints/MainPlayer/Input/IA_RotateObject.IA_RotateObject"));

	if (InputAction.Succeeded())
	{
		RotateAction = InputAction.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> InputAction2(TEXT("/Game/Core/Character/Blueprints/MainPlayer/Input/IA_Touch.IA_Touch"));

	if (InputAction2.Succeeded())
	{
		TouchAction = InputAction2.Object;
	}
}

void UPlacementIndicatorComponent::InitializeComponent()
{
	Super::InitializeComponent();

	AMainPlayerCharacter* OwnerPlayer = Cast<AMainPlayerCharacter>(GetOwner());

	OwnerPlayer->OnInputBindingNotified.AddDynamic(
		this, &ThisClass::BindingInputActions);
}

void UPlacementIndicatorComponent::BindingInputActions(UEnhancedInputComponent* EnhancedInputComponent)
{
	EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Triggered, this, &ThisClass::OnRotate);
	EnhancedInputComponent->BindAction(TouchAction, 
	ETriggerEvent::Triggered, this, &ThisClass::Execute);
}

void UPlacementIndicatorComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                 FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bIsIndicatorActive)
	{
		return;
	}

	if (!PlayerController || !PlayerController->PlayerState)
	{
		return;
	}
	
	AMainPlayerState* PlayerState = Cast<AMainPlayerState>(PlayerController->PlayerState);

	if (!PlayerState)
	{
		return;
	}
	
	AGridManager* GridManager = PlayerState->GetGridManager();

	if (!GridManager)
	{
		return;
	}

	if (IndicateActor && PlayerController->IsLocalController())
	{
		FVector SnappedLocation = GridManager->GetLocationInPointerDirectionPlacement(
			PlayerController, IndicateActor->GetMeshSize());
		
		IndicateActor->SetActorRotation(FMath::RInterpTo(IndicateActor->GetActorRotation(),
		                                                  FRotator(0, GetDegrees(RotateDirection), 0), 0.1f,
		                                                  InterpSpeed));
		FIntVector GridCoord;
		APlacement* PlacedActor;

		bool bIsBlock = GridManager->TryGetPlacement(SnappedLocation, GridCoord, PlacedActor);

		IndicateActor->SetColor(true, bIsBlock);
	}
}

void UPlacementIndicatorComponent::Execute()
{
	Super::Execute();

	if (!bIsIndicatorActive)
	{
		return;
	}

	if (!IndicateActor)
	{
		return;
	}
	
	if (!PlayerController || !PlayerController->PlayerState)
	{
		return;
	}
	
	AMainPlayerState* PlayerState = Cast<AMainPlayerState>(PlayerController->PlayerState);

	if (!PlayerState)
	{
		return;
	}
	
	AGridManager* GridManager = PlayerState->GetGridManager();

	if (!GridManager)
	{
		return;
	}
	
	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(GetOwner());

	const TObjectPtr<UPlayerInventoryComponent> PlayerInventoryComponent = Player->GetPlayerInventoryComponent();

	const int SlotIndex = PlayerInventoryComponent->GetCurrentSlotIndex();

	FItemMetaInfo ItemMetaInfo = PlayerState->GetInventoryComponent()->GetInventoryList()[SlotIndex];
	
	if (PlayerInventoryComponent->RemoveItemCurrentSlotIndex(1))
	{
		if (GetOwner()->GetNetMode() == NM_Standalone)
		{
			const TSubclassOf<APlacement> PlacementFactory = IndicateActor->GetClass();
			// UE_LOG(LogTemp, Warning, TEXT("%s"), *GhostPlacement->GetActorLocation().ToCompactString());
			GridManager->BuildPlacementAtGhost(PlacementFactory, ItemMetaInfo, IndicateActor);
		}
		else
		{
			if (GetOwner()->HasAuthority())
			{
				const TSubclassOf<APlacement> PlacementFactory = IndicateActor->GetClass();
				// UE_LOG(LogTemp, Warning, TEXT("%s"), *GhostPlacement->GetActorLocation().ToCompactString());
				GridManager->BuildPlacementAtGhost(PlacementFactory, ItemMetaInfo, IndicateActor);
			}
			else
			{
				FItemMetaInfo_Net Info(ItemMetaInfo);
				Server_Execute(IndicateActor->GetActorPivotLocation(), IndicateActor->GetActorLocation(), IndicateActor->GetActorRotation(), IndicateActor->GetClass(), Info);
			}
		}
		OnDeactivate();
	}
	
}

void UPlacementIndicatorComponent::ExecuteInternal(FVector Pivot, FVector Location, FRotator Rotation,
	TSubclassOf<APlacement> PlacementClass, FItemMetaInfo ItemMetaInfo)
{
	Super::ExecuteInternal(Pivot, Location, Rotation, PlacementClass, ItemMetaInfo);

	if (!bIsIndicatorActive)
	{
		return;
	}

	if (!PlayerController || !PlayerController->PlayerState)
	{
		return;
	}
	
	AMainPlayerState* PlayerState = Cast<AMainPlayerState>(PlayerController->PlayerState);

	if (!PlayerState)
	{
		return;
	}
	
	AGridManager* GridManager = PlayerState->GetGridManager();

	if (GridManager)
	{
		FItemMetaInfo_Net Info(ItemMetaInfo);
		GridManager->Server_BuildPlacement(PlacementClass, Info, Pivot, Location, Rotation);
	}
}

void UPlacementIndicatorComponent::OnRotate(const FInputActionValue& InputActionValue)
{
	if (!bIsIndicatorActive)
	{
		return;
	}
	
	float Value = InputActionValue.Get<float>();
	
	if (Value > 0)
	{
		RotateDirection = RotateDirection << static_cast<uint8>(1);
	}

	if (Value < 0)
	{
		RotateDirection = RotateDirection >> static_cast<uint8>(1);
	}
}
