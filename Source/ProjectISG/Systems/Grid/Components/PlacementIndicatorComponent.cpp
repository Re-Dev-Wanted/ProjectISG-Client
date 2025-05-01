#include "PlacementIndicatorComponent.h"

#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/PlayerInventoryComponent.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Systems/Grid/Actors/Placement.h"
#include "ProjectISG/Systems/Grid/Manager/GridManager.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"

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

	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	AMainPlayerCharacter* OwnerPlayer = Cast<AMainPlayerCharacter>(GetOwner());

	OwnerPlayer->OnInputBindingNotified.AddDynamic(
		this, &ThisClass::BindingInputActions);

	OwnerPlayer->OnUpdateSelectedItem.AddDynamic(this, &ThisClass::OnChange);
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

	if (PlayerController->GetCharacter()->IsLocallyControlled())
	{
		FVector SnappedLocation = GridManager->GetLocationInPointerDirectionPlacement(
			PlayerController, IndicateActor->GetMeshSize());

		IndicateActor->SetActorLocation(FMath::VInterpTo(IndicateActor->GetActorLocation(), SnappedLocation,
														  0.1f,
														  InterpSpeed));
		
		IndicateActor->SetActorRotation(FMath::RInterpTo(IndicateActor->GetActorRotation(),
		                                                  FRotator(0, GetDegrees(RotateDirection), 0), 0.1f,
		                                                  InterpSpeed));
		FIntVector GridCoord;
		APlacement* PlacedActor;

		bIsBlocked = GridManager->TryGetPlacement(SnappedLocation, GridCoord, PlacedActor);

		IndicateActor->SetOption(true, bIsBlocked);
	}
}

void UPlacementIndicatorComponent::Execute()
{
	if (!bIsIndicatorActive)
	{
		return;
	}
	
	if (bIsBlocked)
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
	
	Server_Execute(IndicateActor->GetActorPivotLocation(),
		IndicateActor->GetActorLocation(), IndicateActor->GetActorRotation(),
		IndicateActor->GetClass(), PlacementItemId);

	if (!bIsInfiniteItem)
	{
		if (PlayerInventoryComponent->RemoveItemCurrentSlotIndex(1))
		{
			OnDeactivate();
		}	
	}
}

void UPlacementIndicatorComponent::ExecuteInternal(FVector Pivot, FVector Location, FRotator Rotation,
	TSubclassOf<APlacement> PlacementClass, uint16 ItemId)
{
	
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
		GridManager->Server_BuildPlacement(PlacementClass, ItemId, Pivot, 
		Location, Rotation);
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

void UPlacementIndicatorComponent::OnChange(
	uint16 ItemId)
{
	const FItemInfoData ItemInfoData = UItemManager::GetItemInfoById(ItemId);

	const bool bIsStructure = UItemManager::IsItemCanHousing(ItemId);

	const TSubclassOf<AActor> ActorClass = ItemInfoData.GetPlaceItemActor();
	
	bIsIndicatorActive = bIsStructure
					&& ActorClass
					&& ActorClass->IsChildOf(APlacement::StaticClass());
	
	if (bIsIndicatorActive)
	{
		// 다른 아이템을 생성하는 도구인지 판단
		bIsInfiniteItem = UItemManager::IsInfiniteDurability(ItemId);
		uint16 OtherItemId = 0;
		TSubclassOf<APlacement> PlacementClass;

		if (bIsInfiniteItem)
		{
			// 다른 아이템Id가 존재하는지 판단
			// 없다면 해당 도구 Data에 문제가 있는것이므로 DataTable 확인
			uint16 Id = UItemManager::GetGeneratedOtherItemIdById(ItemId);

			const FItemInfoData OtherInfoData = UItemManager::GetItemInfoById(Id);
			const bool bIsHousing = UItemManager::IsItemCanHousing(ItemId);

			if (bIsHousing)
			{
				const TSubclassOf<AActor> OtherActorClass = OtherInfoData.GetPlaceItemActor();
				if (OtherActorClass&& OtherActorClass->IsChildOf(APlacement::StaticClass()))
				{
					PlacementClass = OtherActorClass;
					OtherItemId = Id;
				}
			}
		}
		
		PlacementItemId = bIsInfiniteItem && OtherItemId > 0? OtherItemId : ItemId;
		if (!PlacementClass)
		{
			PlacementClass = ActorClass;
		}
		OnActivate(PlacementClass);
	}
	else
	{
		PlacementItemId = 0;
		OnDeactivate();
	}
}

void UPlacementIndicatorComponent::OnActivate(
	const TSubclassOf<APlacement>& Factory)
{
	if (!PlayerController || !PlayerController->PlayerState)
	{
		return;
	}
	
	AMainPlayerState* PlayerState = Cast<AMainPlayerState>(PlayerController->PlayerState);

	if (!PlayerState)
	{
		return;
	}
	
	if (PlayerController->GetCharacter()->IsLocallyControlled())
	{
		AGridManager* GridManager = PlayerState->GetGridManager();

		if (!GridManager)
		{
			return;
		}

		GridManager->SetVisibleGrid(true);
		
		SetActive(true);

		if (IndicateActor)
		{
			APlacement* DestroyActor = IndicateActor.Get();
			DestroyActor->Destroy();
			IndicateActor = nullptr;
		}
	
		IndicateActor = GetWorld()->SpawnActor<APlacement>(Factory);

		if (IndicateActor)
		{
			IndicateActor->SetReplicates(false); // 복제 금지
			IndicateActor->SetActorEnableCollision(false); // 충돌 제거
			IndicateActor->SetActorTickEnabled(false);
			IndicateActor->Setup(GridManager->SnapSize);
		}
	}
}

void UPlacementIndicatorComponent::OnDeactivate()
{
	AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(GetOwner());
	
	if (Player && Player->IsLocallyControlled())
	{
		if (IndicateActor)
		{
			APlacement* DestroyActor = IndicateActor.Get();
			DestroyActor->Destroy();
			IndicateActor = nullptr;
		}

		SetActive(false);
		
		if (PlayerController)
		{
			AMainPlayerState* PlayerState = Cast<AMainPlayerState>(PlayerController->PlayerState);

			if (!PlayerState)
			{
				return;
			}
			
			AGridManager* GridManager = PlayerState->GetGridManager();

			if (GridManager)
			{
				GridManager->SetVisibleGrid(false);
			}
		}
	}
}

void UPlacementIndicatorComponent::Server_Execute_Implementation(FVector Pivot,
	FVector Location, FRotator Rotation, TSubclassOf<APlacement> PlacementClass,
	uint16 ItemId)
{
	ExecuteInternal(Pivot, Location, Rotation, PlacementClass, ItemId);
}
